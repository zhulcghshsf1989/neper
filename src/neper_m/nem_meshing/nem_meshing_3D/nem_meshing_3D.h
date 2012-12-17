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
#include"neut_m.h"
#include"../../structIn_m.h"
#include"../../nem/nem.h"

#include"nem_meshing_3D0.h"

extern int nem_meshing_3D_poly (struct IN, double, struct MULTIM*, double,
  double*, struct GEO, struct NODES*, struct MESH, struct MESH*, int);
