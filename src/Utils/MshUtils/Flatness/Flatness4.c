/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Flatness.h"

void
CalcEltN (struct NODES Nodes, struct MESH Mesh2D, int elt, double *ThisEltN)
{
  int i, n1, n2, n3;
  double *a = oneDDoubleDAlloc (3);
  double *b = oneDDoubleDAlloc (3);
  double norm;

  n1 = Mesh2D.EltNodes[elt][0];
  n2 = Mesh2D.EltNodes[elt][1];
  n3 = Mesh2D.EltNodes[elt][2];

  for (i = 0; i < 3; i++)
  {
    a[i] = Nodes.NodeCoo[n1][i] - Nodes.NodeCoo[n2][i];
    b[i] = Nodes.NodeCoo[n2][i] - Nodes.NodeCoo[n3][i];
  }

  VectorVectProd (a, b, ThisEltN, 0);
  norm = VectorNorm (ThisEltN, 0);
  oneDDoubleScale (ThisEltN, 0, 2, 1 / norm);

  oneDDoubleFree (a);
  oneDDoubleFree (b);

  return;
}
