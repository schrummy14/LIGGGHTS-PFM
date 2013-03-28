// **************************************************************************
//                                  atom.cu
//                             -------------------
//                           W. Michael Brown (ORNL)
//
//  Device code for handling CPU generated neighbor lists
//
// __________________________________________________________________________
//    This file is part of the LAMMPS Accelerator Library (LAMMPS_AL)
// __________________________________________________________________________
//
//    begin                : 
//    email                : brownw@ornl.gov
// ***************************************************************************/

#ifdef NV_KERNEL
#include "lal_preprocessor.h"
#endif

__kernel void kernel_unpack(__global int *dev_nbor, __global int *dev_ij,
                            const int inum, const int t_per_atom) {
  int tid=THREAD_ID_X;
  int offset=tid & (t_per_atom-1);
  int ii=fast_mul((int)BLOCK_ID_X,(int)(BLOCK_SIZE_X)/t_per_atom)+tid/t_per_atom;

  if (ii<inum) {
    __global int *nbor=dev_nbor+ii+inum;
    int numj=*nbor;
    nbor+=inum;
    __global int *list=dev_ij+*nbor;
    __global int *list_end=list+numj;
    list+=offset;
    nbor+=fast_mul(ii,t_per_atom-1)+offset;
    int stride=fast_mul(t_per_atom,inum);
      
    for ( ; list<list_end; list++) {
      *nbor=*list;
      nbor+=stride;
    }
  } // if ii
}

