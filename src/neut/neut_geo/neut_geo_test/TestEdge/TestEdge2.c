/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestEdge.h"

int
TestEdgeLength (struct GEO Geo, int i, int verbosity)
{
  double eps = 1e-15;

  if (Geo.EdgeLength[i] < eps)
  {
    if (verbosity)
      ut_print_message (0, 2, "length is zero.\n");

    return 2;
  }

  return 0;
}

int
TestEdgeReciprocityVer (struct GEO Geo, int i, int verbosity)
{
  int j;
  int ver;

  for (j = 0; j <= 1; j++)
  {
    ver = Geo.EdgeVerNb[i][j];
    if (ut_array_1d_int_eltpos (Geo.VerEdgeNb[ver], Geo.VerEdgeQty[ver], i)
	== -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "based on ver %d, but edge not in ver edge list.\n", ver);

      return 2;
    }
  }

  return 0;
}

int
TestEdgeReciprocityFace (struct GEO Geo, int i, int verbosity)
{
  int j;
  int face;

  /* this test could be more accurate (considering edge ver bounds) */
  if (Geo.EdgeFaceQty[i] < 2)
  {
    if (verbosity)
      ut_print_message (2, 3, "number of faces = %d < 2.\n", Geo.EdgeFaceQty[i]);

    return 2;
  }

  for (j = 0; j < Geo.EdgeFaceQty[i]; j++)
  {
    face = Geo.EdgeFaceNb[i][j];

    if (ut_array_1d_int_eltpos (Geo.FaceEdgeNb[face] + 1, Geo.FaceVerQty[face], i)
	== -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "face %d is in face list, but face not based on edge.\n", face);
      
      return 2;
    }
  }

  return 0;
}
