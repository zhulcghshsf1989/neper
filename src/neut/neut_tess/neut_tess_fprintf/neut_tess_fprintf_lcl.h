/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_tess_fprintf.h"

extern void GEHead (struct TESS, FILE *);
extern void GEFoot (FILE *);
extern void GEVer (struct TESS, FILE *);
extern void GEEdge (struct TESS, FILE *);
extern void GEFace (struct TESS, FILE *);
extern void GEPoly (struct TESS, FILE *);
extern void GEDomain (struct TESS, FILE *);
