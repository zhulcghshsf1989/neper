/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<limits.h>
#include<float.h>
#include<ctype.h>
#include<string.h>
#include"structAll.h"
#include"../structIn_mm.h"
#include"ut.h"

#include"../../neper_t/GermDistrib/RandDistrib/RandDistrib0.h"
#include"../../neper_t/Miscellaneous/Miscellaneous0.h"
#include"../../neper_t/InputData_t/InputData_t0.h"

#include"InputData_mm0.h"

extern void TreatArg_mm (int, char **, int, char **, struct IN *,
			 struct GERMSET*);
extern void SetDefaultOptions_mm (struct IN*, struct GERMSET *);
extern void SetOptions_mm (struct IN *, struct GERMSET *,
			   int, char**);
