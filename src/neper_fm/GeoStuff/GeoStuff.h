/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"structAll.h"
#include"../structIn_fm.h"
#include"AllUtils.h"
#include"ut.h"
#include"orilib.h"
#include"neut.h"

#include"GeoStuff0.h"

extern void WriteStatGeoPoly (FILE*, struct GEO);
extern void WriteStatGeoFace (FILE*, struct GEO);
// extern void WriteStatGeoFaceAngle (FILE*, struct GEO);
extern void WriteStatGeoEdge (FILE*, struct GEO);
extern void WriteStatGeoVer (FILE*, struct GEO);
