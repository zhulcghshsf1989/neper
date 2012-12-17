/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

extern void neut_mesh_fprintf_stellar_nodes (FILE* file, int* BoundNodes, struct NODES Nodes);
extern void neut_mesh_fprintf_stellar_elts (FILE* file, struct MESH Mesh);
