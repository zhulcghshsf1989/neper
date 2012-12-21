/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<gsl/gsl_rng.h>

#include"../structIn_t.h"
#include"neut_t.h"
#include"orilib.h"

#include"net_utils/net_utils0.h"
#include"RandDistrib/RandDistrib0.h"
#include"RegDistrib/RegDistrib0.h"
#include"DefGermSet/DefGermSet0.h"

#include"net_germ0.h"

extern void net_germ_randomize (struct IN, struct GERMSET* pGermSet, struct POLY, char* morpho);
