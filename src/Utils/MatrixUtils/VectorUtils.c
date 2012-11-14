/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"VectorUtils.h"

/* VectProd returns the vectorial product of two vectors A
 * and B into vector C. beg indicates if the vector begins
 * to 0 or 1.
 */
extern void
VectProd (double *A, double *B, double *C, int beg)
{
  if (beg != 0 && beg != 1)
    abort ();

  if (beg == 0)
  {
    C[0] = A[1] * B[2] - A[2] * B[1];
    C[1] = A[2] * B[0] - A[0] * B[2];
    C[2] = A[0] * B[1] - A[1] * B[0];
  }

  if (beg == 1)
  {
    C[1] = A[2] * B[3] - A[3] * B[2];
    C[2] = A[3] * B[1] - A[1] * B[3];
    C[3] = A[1] * B[2] - A[2] * B[1];
  }

  return;
}

/* ScalProd returns the scalar product of two vectors A
 * and B. beg indicates if the vector begins to 0 or 1.
 */
extern double
ScalProd (double *A, double *B, int beg)
{
  double res;

  if (beg == 0)
    res = A[0] * B[0] + A[1] * B[1] + A[2] * B[2];
  else if (beg == 1)
    res = A[1] * B[1] + A[2] * B[2] + A[3] * B[3];
  else
    abort ();

  return res;
}

/* VectNorm returns the norm of the input vector
 * beg indicates if the vector begins to 0 or 1.
 */
extern double
VectNorm (double *A, int beg)
{
  int i;
  double res = 0;

  if (beg != 0 && beg != 1)
    abort ();

  for (i = beg; i <= beg + 2; i++)
    res += A[i] * A[i];

  res = sqrt (res);

  return res;
}
