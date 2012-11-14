/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"gsl/gsl_sort_int.h"

#include"structAll.h"
#include"ut.h"
#include"../../../neut_geo.h"
#include"../../../neut_structs.h"

#include"TestFace0.h"

extern int TestFaceReciprocityEdge (struct GEO, int);
extern int TestFaceReciprocityVer (struct GEO, int);
extern int TestFaceReciprocityPoly (struct GEO, int);
extern int TestFaceState (struct GEO, int);
extern int TestFaceBound (struct GEO, int);
extern int TestFaceSelfIntersect (struct GEO, int);
