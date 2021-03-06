/* ----------------------------------------------------------------------
   LIGGGHTS - LAMMPS Improved for General Granular and Granular Heat
   Transfer Simulations

   LIGGGHTS is part of the CFDEMproject
   www.liggghts.com | www.cfdem.com

   Christoph Kloss, christoph.kloss@cfdem.com
   Copyright 2009-2012 JKU Linz
   Copyright 2012-     DCS Computing GmbH, Linz

   LIGGGHTS is based on LAMMPS
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   This software is distributed under the GNU General Public License.

   See the README file in the top-level directory.
------------------------------------------------------------------------- */

#ifndef LMP_MULTISPHERE_I_H
#define LMP_MULTISPHERE_I_H

/* ---------------------------------------------------------------------- */

inline double Multisphere::max_r_bound() const
{
    double max_r_bound = 0.;

    for(int i = 0; i < nbody_; i++)
        max_r_bound = MathExtraLiggghts::max(max_r_bound,r_bound_(i));

    MPI_Max_Scalar(max_r_bound,world);

    return max_r_bound;
}

/* ---------------------------------------------------------------------- */

inline void Multisphere::copy_body(int from_local, int to_local)
{
    int tag_from =  id_(from_local);

    customValues_.copyElement(from_local, to_local);

    mapArray_[tag_from] = to_local;
}

/* ---------------------------------------------------------------------- */

inline void Multisphere::remove_body(int ilocal)
{
    /*NL*/// if (screen) fprintf(screen,"deleting body tag %d ilocal %d\n",tag,ilocal);
    mapArray_[id_(ilocal)] = -1;
    if(nbody_ > 1) mapArray_[id_(nbody_-1)] = ilocal;

    //NP do not copy the body if the last body in the array is deleted
    /*if(ilocal < nbody_-1)
        copy_body(nbody_-1,ilocal);*/
    customValues_.deleteElement(ilocal);

    nbody_--;
}

/* ---------------------------------------------------------------------- */

inline void Multisphere::calc_nbody_all()
{
   MPI_Sum_Scalar(nbody_,nbody_all_,world);
}

/* ---------------------------------------------------------------------- */

inline void Multisphere::reset_forces(bool extflag)
{
    /*NL*/ //if (screen) fprintf(screen,"resetting force nbody_ %d\n",nbody_);

    fcm_.setAll(nbody_,0.);
    torquecm_.setAll(nbody_,0.);
    if(extflag) dragforce_cm_.setAll(nbody_,0.);
}

/* ---------------------------------------------------------------------- */

inline int Multisphere::calc_n_steps(int, int body, double *p_ref, double *normalvec, double *v_normal)
{
    double pos_rel[3],dt,dist_normal;
    int ibody,timestep,n_steps;

    //NP skip if atom not in rigid body
    if(body < 0)
        return -1;

    //NP body ID stored in atom is global
    //NP need to know where stored in my data
    ibody = map(body);

    //NP get data
    timestep = update->ntimestep;
    dt = update->dt;

    //NP error if body not owned by this proc
    if(ibody < 0)
        error->one(FLERR,"Illegal situation in FixMultisphere::calc_n_steps");

    //NP return if step already set
    if(start_step_(ibody) >= 0)
        return (start_step_(ibody) - timestep);

    //NP calculate number of steps
    vectorSubtract3D(p_ref,xcm_(ibody),pos_rel);
    dist_normal = vectorDot3D(pos_rel,normalvec);
    n_steps = static_cast<int>(dist_normal/(vectorMag3D(v_normal)*dt));
    start_step_(ibody) = n_steps + timestep;
    v_integrate_.set(ibody,v_normal);

    /*NL*///if (screen) fprintf(screen,"set step for body %d to %d\n",ibody,start_step_(ibody));
    /*NL*///error->all(FLERR,"set step");

    return n_steps;
}

/* ---------------------------------------------------------------------- */

inline void Multisphere::release(int iatom,int body,double *v_toInsert,double *omega_toInsert)
{
    int ibody;

    //NP skip if atom not in rigid body
    if(body < 0)
        return;

    //NP body ID stored in atom is global
    //NP need to know where stored in my data
    ibody = map(body);
    if(ibody < 0)
        return;

    bigint step = update->ntimestep;

    if(start_step_(ibody) >= 0 && step >= start_step_(ibody))
    {
        /*NL*/// if (screen) fprintf(screen,"releasing body %d at step " BIGINT_FORMAT "\n",body,update->ntimestep);

        // set v and omega
        vcm_.set(ibody,v_toInsert);
        omega_.set(ibody,omega_toInsert);
        start_step_.set(ibody,-1);
    }
}

#endif
