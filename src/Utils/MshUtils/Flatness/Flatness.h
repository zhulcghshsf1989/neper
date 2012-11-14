/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#define verbosity 0

#include<stdio.h>
#include<stdlib.h>

#include"structAll.h"

#include"../../FileUtils/FileUtils0.h"
#include"../../DisplayUtils/DisplayUtils0.h"
#include"../../ArrayUtils/ArrayUtils0.h"
#include"../../VectorUtils/VectorUtils0.h"
#include"../../NumUtils/NumUtils0.h"
#include"../../CharUtils/CharUtils0.h"
#include"ut.h"

#include"Flatness0.h"

extern void IniFaceEltN (struct NODES, struct MESH, int, double **);
extern void CalcEltN (struct NODES, struct MESH, int, double *);
extern double ThisFaceFF (struct NODES, struct MESH, int, int);
extern int *ReadMod (char *);
extern int NeighElts (struct MESH, int, int);
