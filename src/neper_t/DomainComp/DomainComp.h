/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<gsl/gsl_rng.h>

#include"structAll.h"
#include"../structIn_t.h"
#include "ut.h"
#include "orilib.h"

#include"CubeDomain/CubeDomain0.h"
#include "../IntePara/IntePara0.h"
#include "../PolyComp/PolyAlloc/PolyAlloc0.h"
#include "../../Utils/TessUtils/P2Pm/P2Pm0.h"
#include "../../Utils/TessUtils/ZerosStruct0.h"
#include "../../Utils/TessUtils/FreeStruct0.h"
#include "../PolyComp/PolyDet/PolyCalc/PolyCalc0.h"
#include "../PolyComp/PolyReg/PolyReg0.h"
#include "../PolyComp/PolyFree/PolyFree0.h"

extern void net_domain_cylinder_planes (double, double, int, double**);
extern void net_domain_clip (struct POLY*, double**, int);
extern void net_domain_tesspoly_planes (struct GEO, int, int*, double**);

#include"DomainComp0.h"
