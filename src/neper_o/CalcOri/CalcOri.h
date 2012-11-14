/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include<gsl/gsl_rng.h>

#include"../structIn_o.h"
#include"ut.h"
#include"neut.h"
#include"orilib.h"

#include"CalcOri0.h"

extern int neo_init_ori (FILE*, struct ORI*);
