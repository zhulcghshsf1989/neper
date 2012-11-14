/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void neut_tess_boundingbox (struct TESS, double**);
extern void neut_tess_volume (struct TESS, double*);
extern void neut_tess_domainface (struct TESS, int **);
extern void neut_tess_domainfacever (struct TESS, int **);
extern int neut_tess_poly_true (struct TESS, int);
extern int neut_tess_ver_true (struct TESS, int*, int);
extern int neut_tess_edge_true (struct TESS, int*, int);
extern void neut_tess_edge_poly (struct TESS, int, int*);
extern int neut_tess_face_true (struct TESS, int*, int);

extern void neut_tess_set_zero (struct TESS*);
extern void neut_tess_free (struct TESS*);
extern void neut_tess_init_polytrue (struct TESS*);
extern int neut_tess_face_area (struct TESS, int, double*);
extern int neut_tess_poly_volume (struct TESS, int, double*);

extern int neut_tess_poly_body (struct TESS, int);

extern void neut_tess_init_polybody (struct TESS*);
extern void neut_tess_poly_switch (struct TESS*, int, int);

#include "neut_tess_deform/neut_tess_deform0.h"
#include "neut_tess_geo/neut_tess_geo0.h"
