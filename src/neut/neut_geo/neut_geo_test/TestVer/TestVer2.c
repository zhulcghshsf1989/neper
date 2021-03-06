/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestVer.h"

/* we check is the number of edge is >= 3
 * if it does not have two identical edges
 * and if the edges are useful.
 */
int
TestVerEdgeQtyNNb (struct GEO Geo, int i, int verbosity)
{
  int j;
  int edge;
  int res;

  if (Geo.VerEdgeQty[i] < 3)
  {
    if (verbosity)
      ut_print_message (2, 3, "Number of edges = %d < 3.\n", i, Geo.VerEdgeQty[i]);

    return 1;
  }

  res = Geo.VerEdgeQty[i] - 1;
  ut_array_1d_int_uniq (Geo.VerEdgeNb[i], &res);
  if (res != Geo.VerEdgeQty[i] - 1)
  {
    if (verbosity)
      ut_print_message (2, 3, "2 identical edges in edge list.\n");

    return 1;
  }

  for (j = 0; j < Geo.VerEdgeQty[i]; j++)
  {
    edge = Geo.VerEdgeNb[i][j];
    if (Geo.EdgeState[edge] == -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "edge %d is in edge list, but also marked as removed.\n",
	    i, edge);

      return 1;
    }
  }

  return 0;
}

/* we check the reciprocity of each ver edge */
int
TestVerEdgeReciprocity (struct GEO Geo, int i, int verbosity)
{
  int j;
  int edge;

  for (j = 0; j <= Geo.VerEdgeQty[i] - 1; j++)
  {
    edge = Geo.VerEdgeNb[i][j];
    if (ut_array_1d_int_eltpos (Geo.EdgeVerNb[edge], 2, i) == -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "edge %d is in edge list, but edge not based on ver.\n",
	    edge);

      return 1;
    }
  }

  return 0;
}

// Chacking that the ver is at the right place on the domain
int
TestVerBoundNCoo (struct GEO Geo, int ver, int verbosity)
{
  int i;
  int qty;
  int* face = NULL;
  double dist;

  neut_geo_ver_domfaces (Geo, ver, &face, &qty);

  for (i = 0; i < qty; i++)
  {
    ut_space_point_plane_dist (Geo.VerCoo[ver], Geo.DomFaceEq[face[i]], &dist);
    if (dist > 1e-8)
    {
      if (verbosity)
	ut_print_message (2, 3, "belongs to domain face %d, but does not lie on the face plane (dist = %.g > 1e-8)\n", face[i], dist);

      return 1;
    }
  }

  ut_free_1d_int (face);

  return 0;
}

int
TestVerFaceCoplaneity (struct GEO Geo, int ver, int verbosity)
{
  int i, j, k;
  int edge1, edge2;
  int face;
  int common;

  for (i = 0; i < Geo.VerEdgeQty[ver] - 1; i++)
  {
    edge1 = Geo.VerEdgeNb[ver][i];

    for (j = i + 1; j < Geo.VerEdgeQty[ver]; j++)
    {
      edge2 = Geo.VerEdgeNb[ver][j];

      common = 0;
      for (k = 0; k < Geo.EdgeFaceQty[edge1]; k++)
      {
	face = Geo.EdgeFaceNb[edge1][k];
	if (ut_array_1d_int_eltpos
	    (Geo.EdgeFaceNb[edge2], Geo.EdgeFaceQty[edge2], face) != -1)
	  common++;
      }

      if (common > 1)
      {
	if (verbosity)
	  ut_print_message (2, 3, "has edges %d and %d, but these edges share more than 1 (= %d) face.\n", edge1, edge2, common);

	return 2;
      }
    }
  }

  return 0;
}
