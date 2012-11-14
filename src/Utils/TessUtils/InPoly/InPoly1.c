/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "InPoly.h"

/* InPoly returns 1 if the point whose coordinates are given into coo
 * belongs to polyhedron nb of tessellation Tess. The 
 */
int
InPoly (double *coo, struct TESS Tess, int nb)
{
  int i, j;
  double *faceeq = ut_alloc_1d (4);

  for (i = 1; i <= Tess.PolyFaceQty[nb]; i++)
  {
    for (j = 0; j < 4; j++)
      faceeq[j] =
	(double) Tess.PolyFaceOri[nb][i] *
	Tess.FaceEq[Tess.PolyFaceNb[nb][i]][j];

    if (Side_tol (faceeq, coo, 1e-12) > 0)
      return 0;
  }

  ut_free_1d (faceeq);

  return 1;
}
