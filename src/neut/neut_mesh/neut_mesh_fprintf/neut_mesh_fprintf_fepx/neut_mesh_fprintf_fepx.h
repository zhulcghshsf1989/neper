/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"orilib.h"
#include"neut.h"
#include"neut_mesh_fprintf_fepx0.h"

extern void neut_mesh_fprintf_fepx_parms (FILE *, struct NODES, struct MESH);
extern void neut_mesh_fprintf_fepx_nodes (FILE *, struct NODES);
extern void neut_mesh_fprintf_fepx_elts (FILE *, struct GEO, struct NSET, struct MESH);
extern void neut_mesh_fprintf_fepx_skinelts (FILE*, struct GEO, struct MESH, struct MESH, struct NODES, struct NSET, char*);
extern void neut_mesh_fprintf_fepx_elsets (FILE*, struct MESH);
extern void neut_mesh_fprintf_fepx_nsets (FILE*, struct NSET, struct NSET, struct NSET, char*);
extern void neut_mesh_fprintf_fepx_nset (FILE*, char*, int, int*);
extern void neut_mesh_surffile_update (FILE*, FILE*, struct PART);
extern void neut_mesh_bcsfile_update (FILE*, FILE*, struct PART);
