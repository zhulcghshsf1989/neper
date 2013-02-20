/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "../neut_structs.h"
#include "neut_poly_polymod/neut_poly_polymod0.h"

extern void neut_poly_bbox (struct POLY, double*);
extern int neut_poly_point_in (struct POLY, double*);
extern void neut_poly_free (struct POLY*);
extern void neut_poly_array_free (struct POLY** pPoly, int size);
extern void neut_poly_set_zero (struct POLY*);
