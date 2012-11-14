/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Flatness.h"

void
IniFaceEltN (struct NODES Nodes, struct MESH Mesh2D,
	     int face, double **FaceEltN)
{
  int i, elt;

  for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
  {
    elt = Mesh2D.Elsets[face][i];
    CalcEltN (Nodes, Mesh2D, elt, FaceEltN[i]);
  }

  return;
}

/* NeighElts returns 1 if elt1 and elt2 of Mesh2D are neighbours, 0 else. */
int
NeighElts (struct MESH Mesh2D, int elt1, int elt2)
{
  int i, node;
  int neigh;

  int NodesPerElt = 3 * Mesh2D.EltOrder;

  neigh = 0;
  for (i = 0; i <= NodesPerElt - 1; i++)
  {
    node = Mesh2D.EltNodes[elt1][i];
    if (oneDIntEltPos
	(Mesh2D.EltNodes[elt2], 0, NodesPerElt - 1, node, 0) != -1)
    {
      neigh = 1;
      break;
    }
  }

  return neigh;
}
