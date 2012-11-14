/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"orilib.h"
#include"neut.h"

#include"net_stat0.h"

extern void WriteStatGeoVer (FILE*, char*, struct GEO);
extern void WriteStatGeoEdge (FILE*, char*, struct GEO);
extern void WriteStatGeoFace (FILE*, char*, struct GEO);
extern void WriteStatGeoPoly (FILE*, char*, struct GEO);
