/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_GMSH0_H
#define NEUT_GMSH0_H

extern void neut_gmsh_call (char *, char *);
extern void neut_gmsh_rc (char *);
extern int neut_gmsh_elt_nb (char*, int, int, int*);
extern int neut_gmsh_meshalgo2d_nb (char*, int*);
extern int neut_gmsh_meshalgo3d_nb (char*, int*);
extern int neut_gmsh_optialgo_nb (char*, int*);
extern int neut_gmsh_sphere_tess_fprintf (FILE*, double, double);

#endif /* NEUT_GMSH0_H */
