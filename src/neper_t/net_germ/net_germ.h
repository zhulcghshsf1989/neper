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
#include"net_germ_reg/net_germ_reg0.h"
#include"net_germ_rand/net_germ_rand0.h"
#include"net_germ_neigh/net_germ_neigh0.h"

#include"net_germ0.h"

extern void net_germ_prop (struct IN In, struct TESS Tess, int poly, struct GERMSET* pGSet);
extern void net_germ_randomize (struct IN, struct GERMSET* pGermSet, struct POLY);
extern void net_germ_seed (int level, int poly, char* width, struct GERMSET* pGSet);
