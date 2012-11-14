/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceNodes.h"

void
ThisFaceNodes (struct MESH Mesh2D, int face, int** pdata)
{
  int i, j, elt, eltnodeqty;

  eltnodeqty = neut_elt_nodeqty ("tri", Mesh2D.Dimension, Mesh2D.EltOrder);
  (*pdata) = ut_alloc_1d_int (Mesh2D.Elsets[face][0] * eltnodeqty + 1);
  for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
  {
    elt = Mesh2D.Elsets[face][i];
    for (j = 0; j < eltnodeqty; j++)
      (*pdata)[++(*pdata)[0]] = Mesh2D.EltNodes[elt][j];
  }

  gsl_sort_int ((*pdata) + 1, 1, (*pdata)[0]);

  (*pdata)[0] = oneDIntCompress ((*pdata), 1, (*pdata)[0]);

  return;
}
