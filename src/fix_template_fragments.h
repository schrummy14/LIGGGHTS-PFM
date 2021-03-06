/* ----------------------------------------------------------------------
   LIGGGHTS - LAMMPS Improved for General Granular and Granular Heat
   Transfer Simulations

   LIGGGHTS is part of the CFDEMproject
   www.liggghts.com | www.cfdem.com

   Department for Particule Flow Modelling
   Copyright 2014- JKU Linz

   LIGGGHTS is based on LAMMPS
   LAMMPS - Large-scale Atomic/Molecular Massively Parallel Simulator
   http://lammps.sandia.gov, Sandia National Laboratories
   Steve Plimpton, sjplimp@sandia.gov

   This software is distributed under the GNU General Public License.

   See the README file in the top-level directory.
------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------
   Contributing authors:
   Daniel Queteschiner (JKU Linz)
------------------------------------------------------------------------- */

#ifdef FIX_CLASS

FixStyle(particletemplate/fragments,FixTemplateFragments)

#else

#ifndef LMP_FIX_TEMPLATE_FRAGMENTS_H
#define LMP_FIX_TEMPLATE_FRAGMENTS_H

#include <map>
#include <vector>
#include "fix.h"
#include "fix_template_sphere.h"

namespace LAMMPS_NS {

class PrimitiveWall;
class TriMeshContacts;

enum {
 NORMAL_MODEL_HERTZ_BREAK = 7,
 NORMAL_MODEL_HOOKE_BREAK = 8
};

enum {
  BD_POS_X = 0,
  BD_POS_Y,
  BD_POS_Z,
  BD_SCALED_V_X,
  BD_SCALED_V_Y,
  BD_SCALED_V_Z,
  BD_RADIUS,
  BD_BREAKAGE_PROBABILITY,
  BD_BREAKER_TAG,
  BD_FRAGMENTATION_ENERGY,
  BD_BREAKER_DELTA_MAX,
  BD_SIZE
};

class FixTemplateFragments : public FixTemplateSphere {
 public:

  FixTemplateFragments(class LAMMPS *, int, char **);
  virtual ~FixTemplateFragments();

  virtual void post_create();
  virtual double min_rad() const;
  virtual double max_rad() const;
  virtual double max_r_bound() const;
  virtual int number_spheres();

  // multi insertion
  virtual void init_ptilist(int);
  virtual void randomize_ptilist(int ,int );

  void pre_insert(int n_total, double **breakdata,
                  const std::multimap<int,std::vector<double> > &contacting_atoms_mm,
                  const std::multimap<int,PrimitiveWall*> &prim_walls_mm,
                  const std::multimap<int,TriMeshContacts*> &meshes_mm);

 protected:

  // number of spheres in template
  int nspheres;

  // coords of each sphere with respect to center of mass
  double **x_sphere;

  // radius of each sphere
  double *r_sphere;

  // scale factor if read from a file
  double scale_fact;

  // bounding box
  double x_min[3], x_max[3];

  // bounding sphere - radius and coordinates with respect to com
  double r_bound;
  double x_bound[3];

  // radius of sphere with equal volume
  double r_equiv;

  // number of tries for mc
  int ntry;
  int max_type;
  int maxattempt;

  class PairGran *pair_gran;
  int dnum;
  double rad_min_pct;
  double rad_omit;
  bool omit_post_generation;
  double t10_max;
  std::map<double, double> radiiMassFractions;
  std::vector<std::vector<double> > r_sphere_list;
  std::vector<std::vector<std::vector<double> > > x_sphere_list;
  std::vector<double> collision_factor;

  double elastic_energy(const std::vector<double> &r_sphere, const std::vector<std::vector<double> > &x_sphere);

};

}

#endif
#endif
