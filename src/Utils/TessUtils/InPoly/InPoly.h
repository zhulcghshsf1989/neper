/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"structAll.h"
#include"../../ArrayUtils/ArrayUtils0.h"
#include"../../SpaceUtils/SpaceUtils0.h"
#include"ut.h"

#include"InPoly0.h"

extern void TEHead (FILE *);
extern void TEFoot (FILE *);
extern void TEVer (struct TESS, FILE *);
extern void TEEdge (struct TESS, FILE *);
extern void TEFace (struct TESS, FILE *);
extern void TEPoly (struct TESS, FILE *);
extern void TEDomain (struct TESS, FILE *);
