/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<float.h>
#include<string.h>
#include<assert.h>
#include<math.h>
#include<ctype.h>

#include"structAll.h"
#include"structIn_fm.h"
#include"ut.h"
#include"neut.h"
#include"neper_config.h"

#include"InputData_fm0.h"

extern void TreatArg_fm (int, char **, int, char **, struct IN *,
			 struct GEOPARA *);
extern void SetDefaultOptions_fm (struct IN *, struct GEOPARA *);
extern void SetOptions_fm (struct IN *, struct GEOPARA *,
			   int, char **);
