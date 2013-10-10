/* ----------------------------------------------------------------------
   LIGGGHTS - LAMMPS Improved for General Granular and Granular Heat
   Transfer Simulations

   LIGGGHTS is part of the CFDEMproject
   www.liggghts.com | www.cfdem.com

   This file was modified with respect to the release in LAMMPS
   Modifications are Copyright 2009-2012 JKU Linz
                     Copyright 2012-     DCS Computing GmbH, Linz

   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   Copyright (2003) Sandia Corporation.  Under the terms of Contract
   DE-AC04-94AL85000 with Sandia Corporation, the U.S. Government retains
   certain rights in this software.  This software is distributed under
   the GNU General Public License.

   See the README file in the top-level directory.
------------------------------------------------------------------------- */

#ifdef COMPUTE_CLASS

ComputeStyle(erotate/multisphere,ComputeERotateMultisphere)

#else

#ifndef LMP_COMPUTE_EROTATE_MULTISPHERE_H
#define LMP_COMPUTE_EROTATE_MULTISPHERE_H

#include "compute_ke_multisphere.h"

namespace LAMMPS_NS {

class ComputeERotateMultisphere : public ComputeKEMultisphere {
 public:
  ComputeERotateMultisphere(class LAMMPS *, int, char **);
  ~ComputeERotateMultisphere();

  double compute_scalar();
};

}

#endif
#endif

/* ERROR/WARNING messages:

E: Illegal ... command

Self-explanatory.  Check the input script syntax and compare to the
documentation for the command.  You can use -echo screen as a
command-line option when running LAMMPS to see the offending line.

E: Compute erotate/sphere requires atom style sphere

Self-explanatory.

*/
