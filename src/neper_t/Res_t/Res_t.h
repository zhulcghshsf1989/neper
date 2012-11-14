/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../structIn_t.h"
#include"ut.h"
#include"neut.h"
#include"PointPoly/PointPoly0.h"
#include"Connectivity/Connectivity0.h"
#include"net_oin/net_oin0.h"

#include"../../neper_fm/GeoStuff/GeoStuff0.h"

#include"Res_t0.h"

extern void DrawPoly (struct TESS, int *, int, FILE *);
extern void GeoTess (struct TESS, FILE *);
extern void Phaseinput (struct GERMSET, struct POLY **, FILE *);
