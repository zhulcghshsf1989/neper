/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_tess_fscanf.h"
#include"neut_tess_fscanf_1p9/neut_tess_fscanf0_1p9.h"
#include"neut_tess_fscanf_1p10/neut_tess_fscanf0_1p10.h"

extern void neut_tess_fscanf_version (FILE*, char*);
extern void neut_tess_fscanf_foot (FILE *);
extern void neut_tess_fscanf_ver (struct TESS*, FILE *);
extern void neut_tess_fscanf_edge (struct TESS*, FILE *);
extern void neut_tess_fscanf_face (struct TESS*, FILE *);
extern void neut_tess_fscanf_poly (struct TESS*, FILE *);
extern void neut_tess_fscanf_domain (struct TESS*, FILE *);
