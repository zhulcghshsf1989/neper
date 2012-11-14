/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_geo.h"
#include"../../../neut_structs.h"

extern int neut_geo_test_dom_def      (struct GEO, int);
extern int neut_geo_test_dom_tessver  (struct GEO, int);
extern int neut_geo_test_dom_tessedge (struct GEO, int);
extern int neut_geo_test_dom_tessface (struct GEO, int);
