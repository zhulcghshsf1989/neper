/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<gsl/gsl_rng.h>

#include"../structIn_t.h"
#include "ut.h"
#include"neut_t.h"
#include "orilib.h"

#include"net_utils/net_utils0.h"
#include"net_domain_cube/net_domain_cube0.h"
#include "../net_tess/net_polycomp/PolyAlloc/PolyAlloc0.h"
#include "../net_tess/net_polycomp/PolyDet/PolyCalc/PolyCalc0.h"
#include "../net_tess/net_polycomp/PolyReg/PolyReg0.h"
#include "../net_tess/net_polycomp/PolyFree/PolyFree0.h"

extern void net_domain_cylinder_planes (double, double, int, double**);
extern void net_domain_clip (struct POLY*, double**, int);
extern void net_domain_tesspoly_planes (struct TESS, int, int*, double**);

#include"net_domain0.h"
