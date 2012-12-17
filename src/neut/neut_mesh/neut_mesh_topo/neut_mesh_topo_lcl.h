/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<math.h>
#include"neut.h"

extern int neut_mesh_skinelt_fod (struct NODES Nodes, struct MESH Mesh, int elt,
		       double*** pfodeq, int* pfodqty, int* pfod,
		       int* pdir);
