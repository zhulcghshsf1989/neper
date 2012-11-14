/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef MATRIXUTILS0_H
#define MATRIXUTILS0_H

extern double MatrixDeterminant (int, double **);
extern void Comatrix (int, double **, double **);
extern void TransposeMatrix (int, double **, double **);
extern void MultiplyMatrix (int, double **, double, double **);
extern void InverseMatrix (int, double **, double **);
extern void MatrixByVector (int, double **, double *, double *);
extern void SolveMatrix (int, double **, double *, double *);
extern void GaussJordan (int, double **, double *, double *);

#endif /* MATRIXUTILS0_H */
