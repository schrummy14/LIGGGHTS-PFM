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

/* ----------------------------------------------------------------------
   Contributing authors:
   Christoph Kloss (JKU Linz, DCS Computing GmbH, Linz)
   Daniel Queteschiner (JKU Linz)
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(break/particle,FixBreakParticle)

#else

#ifndef LMP_FIX_BREAK_PARTICLE_H
#define LMP_FIX_BREAK_PARTICLE_H

#include "fix_insert.h"
#include <map>
#include <vector>
#include "contact_interface.h"

namespace LAMMPS_NS {

class FixBreakParticle : public FixInsert {
 public:

  FixBreakParticle(class LAMMPS *, int, char **);
  ~FixBreakParticle();

  void post_create();
  void pre_delete(bool unfixflag);
  virtual void pre_force(int);

  virtual int setmask();
  virtual void init();
  void init_defaults();
  int calc_ninsert_this();
  virtual void end_of_step();

 protected:

  // inherited functions
  virtual void calc_insertion_properties();
  virtual void pre_insert();
  int is_nearby(int);
  void x_v_omega(int ninsert_this,int &ninserted_this, int &ninserted_spheres_this, double &mass_inserted_this);
  double insertion_fraction();

  // functions declared in this class
  inline void generate_random(double *pos, double rad_broken,double rad_insert);
  void print_stats_breakage_during();

  void check_energy_criterion();
  void check_force_criterion();
  void check_von_mises_criterion();

  // per breakage flag
  class FixPropertyAtom *fix_break;
  class FixPropertyAtom *fix_breaker;
  class FixPropertyAtom *fix_breaker_wall;
  class FixPropertyAtom *fix_collision_factor;
  class FixPropertyAtom *fix_stress;

  // template holding data of the fragments
  class FixTemplateFragments *fix_fragments;

  // threshold value
  double threshold;
  int breakage_criterion;
  double fMat;
  double min_break_rad;

  // stats for breakage
  int n_break,n_break_this,n_break_this_local;
  double mass_break,mass_break_this,mass_break_this_local;

  // data of particles to be broken
  double **breakdata;
  class PairGran *pair_gran;
  int dnum;
  int deltaMaxOffset;
  int siblingOffset;
  int collisionFactorOffset;
  int impactEnergyOffset;
  int forceMaxOffset;
  int normalOffset;
  int tag_max;

  double virtual_force(int i, int j, double *siblingDeltaMax);
  double virtual_collision(LIGGGHTS::ContactModels::CollisionData & cdata, double *siblingDeltaMax = NULL);
  void virtual_initial_integrate(int i, std::vector<double> &virtual_v, std::vector<double> &virtual_x);
  void virtual_final_integrate(int i, std::vector<double> &virtual_v);
  void invert_vector(std::vector<double> &v3);

  std::map<int, std::vector<double> > delta_v;
  std::vector<double> virtual_x_i;
  std::vector<double> virtual_x_j;
  std::vector<double> virtual_v_i;
  std::vector<double> virtual_v_j;
  std::vector<double> virtual_f_ij;
};

}

#endif
#endif
