/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"ut.h"
#include"../../../neut_tess.h"
#include"../../../neut_structs.h"

extern int neut_tess_test_dom_def      (struct TESS, int);
extern int neut_tess_test_dom_tessver  (struct TESS, int);
extern int neut_tess_test_dom_tessedge (struct TESS, int);
extern int neut_tess_test_dom_tessface (struct TESS, int);
