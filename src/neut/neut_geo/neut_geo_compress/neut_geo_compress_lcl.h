/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"
#include"../neut_geo_compress.h"

extern void neut_geo_compress_moveface (struct GEO*, int, int);
extern void neut_geo_compress_moveedge (struct GEO*, int, int);
extern void neut_geo_compress_movever  (struct GEO*, int, int);
