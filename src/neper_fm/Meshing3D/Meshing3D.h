/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>

#ifdef HAVE_LIBMATHEVAL
#include<matheval.h>
#endif

#include"ut.h"
#include"neut.h"
#include"../structIn_fm.h"
#include"../nefm/nefm.h"

#include"Meshing3D0.h"

extern int Meshing3D_poly_cl (struct GEOPARA, struct GEO, int, double*);

extern int Meshing3D_poly (struct IN, double, struct MULTIM*, double,
  double*, struct GEO, struct NODES*, struct MESH, struct MESH*, int);
