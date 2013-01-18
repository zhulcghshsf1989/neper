/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMESHPARA_H
#define STRUCTMESHPARA_H

struct MESHPARA
{
  double cl;	 // characteristic lengths of the elts
  double clmin;  // minimim value of cl
  double pl;	 // value of -pl, 

  // The following variables are used to define cl from the values
  // passed to options -rcl, -cl3 or -cl3, but they are not used in the
  // meshing process, where only cl is used (also with clmin and pl).
  // rcl, cl3 and rcl3 are used before meshing, to define cl. rcl is
  // relative to the polyhedron volume. cl3 provides 3 different values
  // in the 3 directions of space, therefore is used in functions
  // nem_scaling_*.  rcl3 is used to define cl3.

  int cltype;	 // type of cl passed as option: cl, 1: rcl, 2: cl3, 3: rcl3
  double rcl;    // relative characteristic lengths of the elts
                 // (relative to the average poly volume)
  double* cl3;   // characteristic lengths of the elts in the 3
                 // directions of space
  double* rcl3;  // same as cl3, but relative to the average poly volume

  // These variables define the domain boudary 

  char* dbound;     // domain boundary definition: "true>2", "body>1", ... 
  double dboundcl;  // value of cl for the domain boundary polys
  double dboundrcl; // value of rcl for the domain boundary polys
  double dboundpcl; // value of pl for the domain boundary polys
}; typedef struct MESHPARA MESHPARA;

#endif /* STRUCTMESHPARA_H */

