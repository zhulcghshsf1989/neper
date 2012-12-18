/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_tess.h"
#include"../../../neut_structs.h"

#include"TestVer0.h"

extern int TestVerEdgeQtyNNb (struct TESS, int, int);
extern int TestVerEdgeReciprocity (struct TESS, int, int);
extern int TestVerBoundNCoo (struct TESS, int, int);
extern int TestVerFaceCoplaneity (struct TESS, int, int);
