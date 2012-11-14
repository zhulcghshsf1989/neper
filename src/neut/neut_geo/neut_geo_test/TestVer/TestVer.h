/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"structAll.h"
#include"ut.h"
#include"../../../neut_geo.h"
#include"../../../neut_structs.h"

#include"TestVer0.h"

extern int TestVerEdgeQtyNNb (struct GEO, int, int);
extern int TestVerEdgeReciprocity (struct GEO, int);
extern int TestVerBoundNCoo (struct GEO, int);
extern int TestVerFaceCoplaneity (struct GEO, int);
