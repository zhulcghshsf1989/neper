/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_tess.h"
#include"../../../neut_structs.h"

#include"TestFace0.h"

extern int TestFaceReciprocityEdge (struct TESS, int, int);
extern int TestFaceReciprocityVer (struct TESS, int, int);
extern int TestFaceReciprocityPoly (struct TESS, int, int);
extern int TestFaceState (struct TESS, int, int);
extern int TestFaceBound (struct TESS, int, int);
extern int TestFaceSelfIntersect (struct TESS, int, int);
