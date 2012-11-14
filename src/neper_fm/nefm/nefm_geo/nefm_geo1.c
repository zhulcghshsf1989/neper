/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nefm_geo.h"

void
nefm_geo_face_1delts (struct GEO Geo, struct MESH Mesh1D, int
			     face, int** pelts, int* peltqty)
{
  int i, j, EdgeQty, edge, ori;

  (*peltqty) = 0;

  EdgeQty = Geo.FaceVerQty[face];
  for (i = 1; i <= EdgeQty; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    (*peltqty) += Mesh1D.Elsets[edge][0];
  }
  
  (*pelts) = ut_alloc_1d_int (*peltqty);
  (*peltqty) = 0;

  for (i = 1; i <= EdgeQty; i++)
  {
    ori = Geo.FaceEdgeOri[face][i];
    edge = Geo.FaceEdgeNb[face][i];

    if (ori == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
	(*pelts)[(*peltqty)++] = Mesh1D.Elsets[edge][j];
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
	(*pelts)[(*peltqty)++] = -Mesh1D.Elsets[edge][j];
  }

  return;
}
