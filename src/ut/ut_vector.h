/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2011, Romain Quey. */
/* See the COPYING file in the top-level directory. */
 
#ifndef UT_VECTOR0_H
#define UT_VECTOR0_H

extern double ut_vector_norm (double *);
extern double ut_vector_int_norm (int *);
extern double ut_vector_scalprod (double *, double *);
extern void   ut_vector_vectprod (double *, double *, double *);
extern double ut_vector_angle_rad (double *, double *);
extern double ut_vector_angle (double *, double *);
extern double ut_vector_int_angle_rad (int *, int *);
extern double ut_vector_int_angle (int *, int *);
extern void   ut_vector_covar (double**, int, double**, double*);
extern void   ut_vector_uvect (double*, double*);

#endif /* UT_VECTOR0_H */
