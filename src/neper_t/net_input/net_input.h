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
#include"neut_t.h"
// #include"../Miscellaneous/Miscellaneous0.h"

#include"net_input0.h"

extern void net_input_treatargs (int, char**, int, char**, struct IN*, struct REG*);
extern void net_input_options_default (struct IN*, struct REG*);
extern void net_input_options_set (struct IN*, struct REG*, int, char**);
