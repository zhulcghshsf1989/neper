/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void neut_poly_bbox (struct POLY, double*);
extern int neut_poly_point_in (struct POLY, double*);
extern void neut_poly_tess (struct POLY, struct TESS*);
extern void neut_poly_geo (struct POLY, struct GEO*);
extern void neut_poly_free (struct POLY*);
extern void neut_poly_set_zero (struct POLY*);

#include "neut_poly_polymod/neut_poly_polymod0.h"
