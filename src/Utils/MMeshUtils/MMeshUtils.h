/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"structAll.h"
#include"../ArrayUtils/ArrayUtils0.h"
#include"../NumUtils/NumUtils0.h"
#include"../MeshUtils/MeshUtils0.h"
#include"../FileUtils/FileUtils0.h"
#include"../CharUtils/CharUtils0.h"
#include"ut.h"

#include"MMeshUtils0.h"
#include"neper_config.h"

extern void FoDAddEltNode (int **, int, struct MESH, int *, int, int, int,
			   int, int, int);
extern void SearchFoDNodes_cube (struct NODES, int ***);
extern int GetNodeFromElt (struct MESH, int, int, int, int, int);
extern int EltCoo2Id (int, int, int, int, int);
extern void ReadGeofProp (int *, int *, char *);
