/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"neut.h"

extern int neut_mesh3d_elt_slice_tet (struct NODES Nodes, struct MESH
    Mesh, int elt, double* eq, struct NODES* pSNodes, struct MESH*
    pSMesh, int** pelt_newold, int*** pnode_newold, double**
    pnode_fact);

extern int neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH
    Mesh, int elt, double* eq, struct NODES* pSNodes, struct MESH*
    pSMesh, int** pelt_newold, int*** pnode_newold, double**
    pnode_fact);
