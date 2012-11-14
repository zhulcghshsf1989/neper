/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Flatness.h"

double
ThisFaceFF (struct NODES Nodes, struct MESH Mesh2D, int face, int type)
{
  int i, j;
  int elt1, elt2;
  double angle, lff = 0, gff = 0;
  double **FaceEltN;

  FaceEltN = twoDDoubleDAlloc (Mesh2D.Elsets[face][0] + 1, 3);

  IniFaceEltN (Nodes, Mesh2D, face, FaceEltN);

  for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
  {
    elt1 = Mesh2D.Elsets[face][i];

    for (j = i + 1; j <= Mesh2D.Elsets[face][0]; j++)
    {
      elt2 = Mesh2D.Elsets[face][j];

      angle = VectorAngle (FaceEltN[i], FaceEltN[j], 0, 1);

      gff = DoubleMax (angle, gff);

      if (NeighElts (Mesh2D, elt1, elt2) == 1)
	lff = DoubleMax (angle, lff);
    }
  }

  twoDDoubleFree (FaceEltN, Mesh2D.Elsets[face][0] + 1);

  if (type == 0)
    return lff;
  else if (type == 1)
    return gff;
  else
    abort ();
}
