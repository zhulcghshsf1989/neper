/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef VECTORUTILS0_H
#define VECTORUTILS0_H

extern double VectorNorm (double *, int);
extern double VectorScalProd (double *, double *, int);
extern void VectorVectProd (double *, double *, double *, int);
extern double VectorAngle (double *, double *, int, int);

#endif /* VECTORUTILS0_H */
