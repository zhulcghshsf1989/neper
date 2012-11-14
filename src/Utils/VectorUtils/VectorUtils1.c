/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "VectorUtils.h"
#define PI 3.14159265358979323844

/* returns the norm of a vector */
double
VectorNorm (double *a, int beg)
{
  return sqrt (a[beg] * a[beg] + a[beg + 1] * a[beg + 1] +
	       a[beg + 2] * a[beg + 2]);
}

/* returns the scalar product of two vectors */
double
VectorScalProd (double *a, double *b, int beg)
{
  return a[beg] * b[beg] + a[beg + 1] * b[beg + 1] + a[beg + 2] * b[beg + 2];
}

/* returns the vectorial product of two vectors */
void
VectorVectProd (double *a, double *b, double *c, int beg)
{
  c[beg] = a[beg + 1] * b[beg + 2] - a[beg + 2] * b[beg + 1];
  c[beg + 1] = a[beg + 2] * b[beg] - a[beg] * b[beg + 2];
  c[beg + 2] = a[beg] * b[beg + 1] - a[beg + 1] * b[beg];

  return;
}

/* returns the angle between two vectors in radian (type!=1)
 * or degree (type=1)
 */
double
VectorAngle (double *a, double *b, int beg, int type)
{
  double angle;
  double eps = 1e-8;

  angle = VectorScalProd (a, b, beg);
  angle /= VectorNorm (a, beg);
  angle /= VectorNorm (b, beg);
  if (angle < 1 - eps && angle > -1 + eps)
    angle = acos (angle);
  else
    angle = 0;

  if (type == 1)
    angle *= (180 / PI);

  return angle;
}
