/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_randist.h>

#include"structAll.h"
#include"ut.h"
#include"neut.h"

#include"../IntePara/IntePara0.h"
#include"../DomainComp/CubeDomain/CubeDomain0.h"
#include"../PolyComp/PolyComp0.h"
#include"../Tess/Tess0.h"

#include"Miscellaneous0.h"

extern void GermSetGermsPer (struct GERMSET *);
extern void GermSetGermsSD (struct GERMSET *);
extern void GermSet4EmpD (struct GERMSET *);
extern void CalcNeighVol (struct GERMSET *);

