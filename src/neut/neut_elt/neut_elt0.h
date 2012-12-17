/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_ELT0_H
#define NEUT_ELT0_H

extern void neut_elt_name_prop (char *, char *, char *, int *, int *);
extern int neut_elt_nodeqty (char *, int, int);
extern int neut_elt_dim_gtype_order (int, int);
extern int neut_elt_order (char *, int, int);
extern int neut_elt_gtype_prop (int, char*, int*, int*);
extern double neut_elt_rr_3d (double*, double*, double*, double*);
extern double neut_elt_rr_2d (double*, double*, double*);
extern double neut_elt_3d_rho (double*, double*, double*, double*);

#endif /* NEUT_ELT0_H */
