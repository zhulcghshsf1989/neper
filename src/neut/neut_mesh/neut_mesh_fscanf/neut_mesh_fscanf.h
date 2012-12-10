/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"

#include"neut_mesh_fscanf0.h"

extern void ReadNodes (FILE *, struct NODES *, int *);
extern int  ReadNodesHead (FILE *);
extern void ReadNodesProp (FILE *, struct NODES *, int *);
extern void ReadNodesFoot (FILE *);
extern void ReadElts (struct MESH *, struct MESH *, struct MESH *,
		      struct MESH *, FILE *);
extern int  ReadMeshOfDim (FILE *, struct MESH *, int *, int, int);
extern void ReadEltsFoot (FILE *);
extern void ReadEltsProp (FILE *, struct MESH *, int **, int);
extern void SetElsets (struct MESH *, int *, int **);
extern void SetMeshToNull (struct MESH *);
extern void neut_mesh_fscanfHead (FILE*, int*);
