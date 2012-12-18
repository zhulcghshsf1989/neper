/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void neut_tesl_bbox (struct TESL, double**);
extern void neut_tesl_volume (struct TESL, double*);
extern void neut_tesl_domainface (struct TESL, int **);
extern void neut_tesl_domainfacever (struct TESL, int **);
extern int neut_tesl_poly_true (struct TESL, int);
extern int neut_tesl_ver_true (struct TESL, int*, int);
extern int neut_tesl_edge_true (struct TESL, int*, int);
extern void neut_tesl_edge_poly (struct TESL, int, int*);
extern int neut_tesl_face_true (struct TESL, int*, int);

extern void neut_tesl_set_zero (struct TESL*);
extern void neut_tesl_free (struct TESL*);
extern void neut_tesl_init_polytrue (struct TESL*);
extern int neut_tesl_face_area (struct TESL, int, double*);
extern int neut_tesl_poly_volume (struct TESL, int, double*);

extern int neut_tesl_poly_body (struct TESL, int);

extern void neut_tesl_init_polybody (struct TESL*);
extern void neut_tesl_poly_switch (struct TESL*, int, int);

#include "neut_tesl_deform/neut_tesl_deform0.h"
#include "neut_tesl_tess/neut_tesl_tess0.h"
