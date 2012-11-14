/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef SPACEUTILS0_H
#define SPACEUTILS0_H

#include<gsl/gsl_rng.h>

extern double twoDDoubleDist (double *, double *, int);
extern double threeDDoubleDist (double *, double *, int);
extern int Side (double *, double *);
extern int Side_tol (double *, double *, double);
extern int Side2d (double *, double *);

extern double TriangleArea (double *, double *, double *);
extern void Barycenter (double **, int, double *);

extern void DistribPoints (double *, double *, int, double **, gsl_rng *);

extern void GetPtNextPos (double **, double *, double, int, double *);

extern int PointAligned (double *, double *, double *, double);

#endif /* SPACEUTILS0_H */
