/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MMESH0_H
#define UT_MMESH0_H

#include"../neut_structs.h"
extern void neut_mmesh_elset_fscanf (FILE*, struct ELSET*, int);
extern void neut_mmesh_2d_fprintf (struct NODES, struct MESH, char*);
extern void BuildMMesh3DCentre (int sizeX, int sizeY, int sizeZ, int *pEltQty,
		    double ***pEltCoo);
extern void BuildMMesh3D (int *size, int order,
	      struct NODES *pNodes, struct MESH *pMesh, struct NSET*);
extern void SearchFoDNodes_cube (struct NODES Nodes, struct NSET*);
extern int EltCoo2Id (int X, int Y, int Z, int XEQty, int YEQty);

#endif
