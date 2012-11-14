/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"structAll.h"
#include"../../ArrayUtils/ArrayUtils0.h"
#include"../../FileUtils/FileUtils0.h"

#include"PrintGeoAsX0.h"

extern void PrintGeoFaceToPlot (struct GEO, int *, int *);
extern void PrintGeoEdgeToPlot (struct GEO, int *, int *);
extern void PrintGeoVerToPlot (struct GEO, int *, int *);
extern void PrintGeoFaceToPlot_poly (struct GEO, int*, int*);
