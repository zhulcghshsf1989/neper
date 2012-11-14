/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"ut.h"
#include"neut.h"

#include"../../structIn_t.h"

#include"DefGermSet0.h"

extern void GermSetGermsPer (struct GERMSET *);
extern void GermSetGermsSD (struct GERMSET *);
extern void GermSet4EmpD (struct GERMSET *);
extern void CalcNeighVol (struct GERMSET *);

