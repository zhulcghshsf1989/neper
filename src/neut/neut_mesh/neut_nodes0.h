/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_NODES0_H
#define UT_NODES0_H

#include"../neut_structs.h"

extern void neut_nodes_free (struct NODES *);
extern void neut_nodes_scale (struct NODES *, double, double, double);
extern void neut_nodes_shift (struct NODES *, double, double, double);
extern void neut_nodes_domain (struct NODES, double *);
extern void neut_nodes_init_boundingbox (struct NODES*);
extern void neut_nodes_fodnodes (struct NODES, int **);
extern void neut_mesh_prop_fscanf_geof (FILE *, int *, int *);
extern void neut_nodes_set_zero (struct NODES*);
extern void neut_nodes_fprintf_scotch (FILE *, struct NODES);
extern void neut_nodes_fprintf_stellar (FILE *, struct NODES);
extern void neut_nodes_addnode (struct NODES*, double*, double);
extern void neut_nodes_nodes (struct NODES, struct NODES*);
extern void neut_nodes_proj_alongontomesh (struct NODES*, double*, struct NODES, struct MESH, int);
extern int neut_point_proj_alongontomesh (double*, double*, struct NODES, struct MESH, int);

extern void neut_nodes_fprintf_gmsh (FILE*, struct NODES);

extern int neut_nodes_point_closestnode (struct NODES, double*, int*);

extern int  neut_nodes_eltdim (struct MESH, struct MESH,
			       struct MESH, struct MESH, int);

extern int neut_nodes_rmorphans (struct NODES*, struct MESH*, struct NSET*);

#endif
