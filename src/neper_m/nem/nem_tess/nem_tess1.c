/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_tess.h"

void
nem_tess_face_1delts (struct TESS Tess, struct MESH Mesh1D, int
			     face, int** pelts, int* peltqty)
{
  int i, j, EdgeQty, edge, ori;

  (*peltqty) = 0;

  EdgeQty = Tess.FaceVerQty[face];
  for (i = 1; i <= EdgeQty; i++)
  {
    edge = Tess.FaceEdgeNb[face][i];
    (*peltqty) += Mesh1D.Elsets[edge][0];
  }
  
  (*pelts) = ut_alloc_1d_int (*peltqty);
  (*peltqty) = 0;

  for (i = 1; i <= EdgeQty; i++)
  {
    ori = Tess.FaceEdgeOri[face][i];
    edge = Tess.FaceEdgeNb[face][i];

    if (ori == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
	(*pelts)[(*peltqty)++] = Mesh1D.Elsets[edge][j];
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
	(*pelts)[(*peltqty)++] = -Mesh1D.Elsets[edge][j];
  }

  return;
}
