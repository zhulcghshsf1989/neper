/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "FEUtils.h"

int
BaryInterpolation (double **NodeCoo, double *NodesVar, double *Coo,
		   double *pPtVar)
{
  int i, j;
  double *Dist = oneDDoubleDAlloc (5);

  /* Recording distances from the pt to every node
   */
  Dist[0] = 0;
  for (i = 1; i <= 4; i++)
  {
    Dist[i] = twoDDoubleDist (Coo, NodeCoo[i], 0);
    Dist[0] += Dist[i];
  }

  (*pPtVar) = 0;
  for (j = 1; j <= 4; j++)
    (*pPtVar) += (NodesVar[j] * Dist[j]);
  (*pPtVar) /= Dist[0];

  oneDDoubleFree (Dist);

  return 0;
}

int
FEInterpolation (double **NodeCoo, double *NodesVar, double *Coo,
		 double *pPtVar)
{
  double **p = twoDDoubleDAlloc (3, 5);
  double a, b, c;
  double sol1, sol2, X, Y;
  int sol;

  /* Calculating X */
  p[1][1] =
    -NodeCoo[1][0] + NodeCoo[2][0] + NodeCoo[3][0] - NodeCoo[4][0];
  p[1][2] =
    -NodeCoo[1][0] - NodeCoo[2][0] + NodeCoo[3][0] + NodeCoo[4][0];
  p[1][3] = NodeCoo[1][0] - NodeCoo[2][0] + NodeCoo[3][0] - NodeCoo[4][0];
  p[1][4] = NodeCoo[1][0] + NodeCoo[2][0] + NodeCoo[3][0] + NodeCoo[4][0];

  p[2][1] =
    -NodeCoo[1][1] + NodeCoo[2][1] + NodeCoo[3][1] - NodeCoo[4][1];
  p[2][2] =
    -NodeCoo[1][1] - NodeCoo[2][1] + NodeCoo[3][1] + NodeCoo[4][1];
  p[2][3] = NodeCoo[1][1] - NodeCoo[2][1] + NodeCoo[3][1] - NodeCoo[4][1];
  p[2][4] = NodeCoo[1][1] + NodeCoo[2][1] + NodeCoo[3][1] + NodeCoo[4][1];

  a = p[2][1] * p[1][3] - p[1][1] * p[2][3];
  b =
    p[2][1] * p[1][2] - p[1][1] * p[2][2] + (4 * Coo[0] - p[1][4]) * p[2][3] -
    (4 * Coo[1] - p[2][4]) * p[1][3];
  c = p[2][2] * (4 * Coo[0] - p[1][4]) - p[1][2] * (4 * Coo[1] - p[2][4]);

  sol = gsl_poly_solve_quadratic (a, b, c, &sol1, &sol2);

  if (sol == 0)
    abort ();
  else if (sol == 1)
    X = sol1;
  else
  {
    if (sol1 >= -1 && sol1 <= 1)
      X = sol1;
    else
      X = sol2;
  }

  /* Calculating Y */
  a = p[2][2] * p[1][3] - p[1][2] * p[2][3];
  b =
    p[2][2] * p[1][1] - p[2][1] * p[1][2] + (4 * Coo[0] - p[1][4]) * p[2][3] -
    (4 * Coo[1] - p[2][4]) * p[1][3];
  c = p[2][1] * (4 * Coo[0] - p[1][4]) - p[1][1] * (4 * Coo[1] - p[2][4]);

  sol = gsl_poly_solve_quadratic (a, b, c, &sol1, &sol2);

  if (sol == 0)
    abort ();
  else if (sol == 1)
    Y = sol1;
  else
  {
    if (sol1 >= -1 && sol1 <= 1)
      Y = sol1;
    else
      Y = sol2;
  }

  if (X < -1 || X > 1 || Y < -1 || Y > 1)
  {
    ut_print_message (2, 0, "Reference space: point out of range.\n");
    abort ();
    return -1;
  }

  /* Interpolation */
  (*pPtVar) =
    ((1 - X) * (1 - Y) * NodesVar[1] + (1 + X) * (1 - Y) * NodesVar[2] +
     (1 + X) * (1 + Y) * NodesVar[3] + (1 - X) * (1 + Y) * NodesVar[4]) / 4;

  twoDDoubleFree (p, 3);

  return 0;
}
