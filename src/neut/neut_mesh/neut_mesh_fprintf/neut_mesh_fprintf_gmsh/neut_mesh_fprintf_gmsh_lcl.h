/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

extern void neut_meshheader_fprintf_gmsh (FILE* file);
extern void neut_elts_fprintf_gmsh (FILE* file, struct MESH Mesh0D,
    struct MESH Mesh1D, struct MESH Mesh2D, struct MESH Mesh3D, struct
    PART Part, char* dim, char* numbering);
extern void neut_nodes_fprintf_gmsh (FILE* file, struct NODES Nodes);
