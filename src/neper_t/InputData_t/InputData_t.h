/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<float.h>
#include<math.h>
#include<ctype.h>

#include"../structIn_t.h"
#include"ut.h"
#include"neut.h"
#include"../Miscellaneous/Miscellaneous0.h"

#include"InputData_t0.h"

extern void TreatArg_t (int, char **, int, char **, struct IN *,
			struct GERMSET *);
extern void SetDefaultOptions_t (struct IN *, struct GERMSET *);
extern void SetOptions_t (struct IN*, struct GERMSET*, int, char**);
