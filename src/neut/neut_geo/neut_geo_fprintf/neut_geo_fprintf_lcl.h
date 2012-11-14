/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_geo_fprintf.h"

extern void GEHead (struct GEO, FILE *);
extern void GEFoot (FILE *);
extern void GEVer (struct GEO, FILE *);
extern void GEEdge (struct GEO, FILE *);
extern void GEFace (struct GEO, FILE *);
extern void GEPoly (struct GEO, FILE *);
extern void GEDomain (struct GEO, FILE *);
