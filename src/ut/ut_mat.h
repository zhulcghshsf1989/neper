/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_MAT0_H
#define UT_MAT0_H

extern int ut_mat_product (double **, int, int, double **, int, int,
			   double **);
extern int ut_mat_product_33 (double **, double **, double **);
extern double ut_mat_ddot_33 (double **, double **);

extern int ut_mat_vect_product (double **, int, int, double *, int, double *);

extern double ut_mat_det_22 (double **);
extern double ut_mat_det_33 (double **);
extern int ut_mat_inverse_22 (double **, double **);

extern void ut_mat_eigen (int, double **, double *, double **);
extern void ut_mat_transpose (int, double **, double **);
extern void ut_mat_transpose_ns (int, int, double**, double**);
extern void ut_mat_trace (double**, int, double*);

extern int  ut_mat_inverse (double**, int, double**);
extern void  ut_mat_sym (double**, int, double**);
extern void  ut_mat_skew (double**, int, double**);

#endif /* UT_MAT0_H */
