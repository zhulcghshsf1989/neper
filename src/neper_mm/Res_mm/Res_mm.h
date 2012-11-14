/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"ut.h"
#include"neut.h"
#include"../structIn_mm.h"

#include"Res_mm0.h"

extern void CalcPolyEltQty (struct MESH, int**);
extern void WriteMapMeshGmsh (char*, struct NODES, struct MESH*, struct MESH*,
                              struct MESH*, struct MESH*, FILE*);
extern void WriteMapMeshGeof (struct IN, char*, char*, struct NODES, struct MESH , int**, FILE*);
extern void WriteMapMeshAbq (struct IN, char*, char*, struct NODES,
    struct MESH, int**, FILE*);

extern void WriteNSets (int nset, char *nsetlist, int **FoDNodes, char *format,
	    FILE * file);
extern void FoD2EoDNodes (int **FoDNodes, int ***pEoDNodes);
extern void EoD2CoDNodes (int **EoDNodes, int ***pCoDNodes);
extern void FoD2FoDBNodes (int **FoDNodes, int **EoDNodes, int ***pFoDBNodes);
extern void EoD2EoDBNodes (int **EoDNodes, int **CoDNodes, int ***pEoDBNodes);
extern void WriteNSet (char *name, int *NodeSet, char *format, FILE * file);
