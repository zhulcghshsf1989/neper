/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut_vs.h"
// #include"../../nefm/nefm.h"
#include "nevs_print_utils0.h"
#include "../nevs_print_header/nevs_print_header0.h"
#include "../nevs_print_foot/nevs_print_foot0.h"

extern void nevs_print_triangle (FILE*, double*, double*, double*,
    char*, char*, char*);
extern void nevs_print_triangle_edge (FILE*, double*, double*, double*,
    char*, char*);
extern void nevs_print_segment (FILE*, double*, double*, char*,
    char*);
extern void nevs_print_tet_edge (FILE*, double*, double*, double*, double*, char*, char*);
extern void nevs_print_tet (FILE*, double*, double*, double*, double*, char*, char*, char*);
