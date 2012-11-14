/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_geo.h"
#include"../../../neut_structs.h"

#include"TestEdge0.h"

extern int TestEdgeLength (struct GEO, int);
extern int TestEdgeReciprocityVer (struct GEO, int);
extern int TestEdgeReciprocityFace (struct GEO, int);
