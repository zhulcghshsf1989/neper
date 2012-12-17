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

#include"../structIn_o.h"
#include"ut.h"
#include"neut_o.h"

#include"InputData_o0.h"

extern void TreatArg_o (int, char **, int, char **, struct ORI *,
			struct IN *);
extern void SetDefaultOptions_o (struct ORI *, struct IN *);
extern void SetOptions_o (struct ORI*, struct IN*, int, char**);
