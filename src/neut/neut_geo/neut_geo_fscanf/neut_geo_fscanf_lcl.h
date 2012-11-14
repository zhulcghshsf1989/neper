/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_geo_fscanf.h"
#include"neut_geo_fscanf_1p9/neut_geo_fscanf0_1p9.h"

extern void neut_geo_fscanf_version (FILE*, char*);
extern void neut_geo_fscanf_head (struct GEO*, FILE *);
extern void neut_geo_fscanf_foot (FILE *);
extern void neut_geo_fscanf_ver (struct GEO*, FILE *);
extern void neut_geo_fscanf_edge (struct GEO*, FILE *);
extern void neut_geo_fscanf_face (struct GEO*, FILE *);
extern void neut_geo_fscanf_poly (struct GEO*, FILE *);
extern void neut_geo_fscanf_domain (struct GEO*, FILE *);
