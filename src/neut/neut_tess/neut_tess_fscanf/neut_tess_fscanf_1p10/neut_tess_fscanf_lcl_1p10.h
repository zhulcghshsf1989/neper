/* This file is part of the 'neper' program. */
/* Copyright_1p10 (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_tess_fscanf0_1p10.h"

extern void neut_tess_fscanf_version_1p10 (FILE*, char*);
extern void neut_tess_fscanf_head_1p10 (struct TESS*, FILE*);
extern void neut_tess_fscanf_foot_1p10 (FILE *);
extern void neut_tess_fscanf_ver_1p10 (struct TESS*, FILE *);
extern void neut_tess_fscanf_edge_1p10 (struct TESS*, FILE *);
extern void neut_tess_fscanf_face_1p10 (struct TESS*, FILE *);
extern void neut_tess_fscanf_poly_1p10 (struct TESS*, FILE *);
extern void neut_tess_fscanf_domain_1p10 (struct TESS*, FILE *);
