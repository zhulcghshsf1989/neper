/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "SpaceUtils.h"

/* Triangle area returns the area of the triangle
 * whose vertex coordinates are specified as input data:
 * vertex i: coo[i][1,2,3] (i=1,2,3)
 * The used formula is:
 * A = || (P2-P1) vect. (P3-P1) || /2
 */
double
TriangleArea (double *p1, double *p2, double *p3)
{
  int i;
  double res;
  double* tmp1 = ut_alloc_1d (3);
  double* tmp2 = ut_alloc_1d (3);
  double* tmp3 = ut_alloc_1d (3);

  for (i = 0; i < 3; i++)
  {
    tmp1[i] = p2[i] - p1[i];
    tmp2[i] = p3[i] - p1[i];
  }
  VectProd (tmp1, tmp2, tmp3, 0);

  res = 0.5 * VectNorm (tmp3, 0);

  ut_free_1d (tmp1);
  ut_free_1d (tmp2);
  ut_free_1d (tmp3);

  return res;
}

/* PolygonTriangle is a subroutine of PolygonArea. It returns a compacted
 * form of the coordinates of the vertices of a triangle.
void
PolygonTriangle (struct POLYGON Polygon, int v1, int v2, double **coo)
{
  int i;

  for (i = 0; i <= 2; i++)
    {
      coo[1][i] = Polygon.CenterCoo[i];
      coo[2][i] = Polygon.VerCoo[v1][i];
      coo[3][i] = Polygon.VerCoo[v2][i];
    }

  return;
}
 */
