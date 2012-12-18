/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_tess.h"
#include"../../../neut_structs.h"

#include"TestPoly0.h"

extern int TestPolyReciprocityFace (struct TESS, int, int);
extern int TestPolyCoplaneityFace (struct TESS, int, int);
extern int TestPolyOrientationFace (struct TESS, int, int);
