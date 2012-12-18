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

#include"structIn_m.h"
#include"ut.h"
#include"neut_m.h"
#include"neper_config.h"

#include"nem_input0.h"

extern void nem_input_treatargs (int, char **, int, char **, struct IN *,
			 struct TESSPARA *);
extern void nem_input_options_default (struct IN *, struct TESSPARA *);
extern void nem_input_options_set (struct IN *, struct TESSPARA *,
			   int, char **);
