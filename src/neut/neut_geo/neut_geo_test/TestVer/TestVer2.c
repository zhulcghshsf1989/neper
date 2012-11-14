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
    if (verbosity >= 2)
    {
      ut_print_lineheader (1);
      printf ("ver %d has less than 3 = %d edges\n", i, Geo.VerEdgeQty[i]);
    }
    return 1;
  }

  res = Geo.VerEdgeQty[i] - 1;
  ut_array_1d_int_uniq (Geo.VerEdgeNb[i], &res);
  if (res != Geo.VerEdgeQty[i] - 1)
  {
    /*
       ut_print_lineheader(1);
       printf("ver %d has two identical edges\n",i);
     */
    return 1;
  }

  for (j = 0; j <= Geo.VerEdgeQty[i] - 1; j++)
  {
    edge = Geo.VerEdgeNb[i][j];
    if (Geo.EdgeState[edge] == -1)
    {
      /*
         ut_print_lineheader(1);
         printf("ver %d has a useless edge: nb %d\n",i,edge);
       */
      return 1;
    }
  }

  return 0;
}

/* we check the reciprocity of each ver edge */
int
TestVerEdgeReciprocity (struct GEO Geo, int i)
{
  int j;
  int edge;

  for (j = 0; j <= Geo.VerEdgeQty[i] - 1; j++)
  {
    edge = Geo.VerEdgeNb[i][j];
    if (ut_array_1d_int_eltpos (Geo.EdgeVerNb[edge], 2, i) == -1)
    {
      /*
         ut_print_lineheader(2);
         printf("edge %d of ver %d = edge nb %d has not ver %d as ver.\n",j,i,edge,i);
       */
      return 1;
    }
  }

  return 0;
}

// Chacking that the ver is at the right place on the domain
int
TestVerBoundNCoo (struct GEO Geo, int ver)
{
  int i, j;
  int qty;
  int* face = NULL;
  double dist;

  neut_geo_ver_domfaces (Geo, ver, &face, &qty);

  for (i = 0; i < qty; i++)
  {
    ut_space_point_plane_dist (Geo.VerCoo[ver], Geo.DomFaceEq[face[i]], &dist);
    if (dist > 1e-8)
    {
      printf ("ver %d has ", ver);
      printf ("boundary: ");
      ut_array_1d_int_fprintf (stdout, Geo.VerDom[ver], 2, "%d");
      printf ("and faces: ");
      ut_array_1d_int_fprintf (stdout, face, qty, "%d");
      printf ("of equations:\n");
      for (j = 0; j < qty; j++)
	ut_array_1d_fprintf (stdout, Geo.DomFaceEq[face[j]], 4, "%f");
      printf ("coo: ");
      ut_array_1d_fprintf (stdout, Geo.VerCoo[ver], 3, "%f");
      ut_error_reportbug ();
    }
  }

  ut_free_1d_int (face);

  return 0;
}

int
TestVerFaceCoplaneity (struct GEO Geo, int ver)
{
  int i, j, k;
  int edge1, edge2;
  int face;
  int common;

  for (i = 0; i <= Geo.VerEdgeQty[ver] - 2; i++)
  {
    edge1 = Geo.VerEdgeNb[ver][i];

    for (j = i + 1; j <= Geo.VerEdgeQty[ver] - 1; j++)
    {
      edge2 = Geo.VerEdgeNb[ver][j];

      /* printf ("i = %d j = %d; edge1 = %d  edge2 = %d\n", i, j, edge1, edge2); */

      /* searching the number of common faces of edge1 and edge2 */
      common = 0;
      for (k = 0; k <= Geo.EdgeFaceQty[edge1] - 1; k++)
      {
	face = Geo.EdgeFaceNb[edge1][k];
	if (ut_array_1d_int_eltpos
	    (Geo.EdgeFaceNb[edge2], Geo.EdgeFaceQty[edge2], face) != -1)
	  common++;
      }

      if (common > 1)
	return 2;
    }
  }

  return 0;
}
