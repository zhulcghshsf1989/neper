/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestFace.h"

int
TestFaceReciprocityEdge (struct GEO Geo, int i, int verbosity)
{
  int j;
  int edge;

  if (Geo.FaceVerQty[i] < 3)
  {
    if (verbosity)
      ut_print_message (2, 3, "Number of vertices/edges = %d < 3.\n", i, Geo.FaceVerQty[i]);

    return 2;
  }

  for (j = 1; j <= Geo.FaceVerQty[i]; j++)
  {
    edge = Geo.FaceEdgeNb[i][j];
    if (ut_array_1d_int_eltpos
	(Geo.EdgeFaceNb[edge], Geo.EdgeFaceQty[edge], i) == -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "based on edge %d, but face not in edge face list.\n", edge);

      return 2;
    }
  }

  return 0;
}

int
TestFaceReciprocityVer (struct GEO Geo, int i, int verbosity)
{
  int j;
  int ver;
  int faceqty;
  int* face = NULL;

  for (j = 1; j <= Geo.FaceVerQty[i]; j++)
  {
    ver = Geo.FaceVerNb[i][j];
    neut_geo_ver_faces (Geo, ver, &face, &faceqty);
    if (ut_array_1d_int_eltpos (face, faceqty, i) == -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "based on ver %d, but face not found in ver faces.\n", ver);

      return 2;
    }
  }

  ut_free_1d_int (face);

  return 0;
}

int
TestFaceReciprocityPoly (struct GEO Geo, int i, int verbosity)
{
  int j;
  int poly;

  if (Geo.FacePoly[i][0] < 0)
  {
    if (verbosity) 
      ut_print_message (2, 3, "first poly have a negative id (%d).\n",
	  Geo.FacePoly[i][0]);

    return 2;
  }

  if (Geo.FacePoly[i][0] < 0 && Geo.FacePoly[i][1] < 0)
  {
    if (verbosity) 
      ut_print_message (2, 3, "both polys have negative ids (%d, %d).\n",
	  Geo.FacePoly[i][0], Geo.FacePoly[i][1]);

    return 2;
  }

  for (j = 0; j < 2; j++)
  {
    poly = Geo.FacePoly[i][j];

    if (poly > 0)
      if (ut_array_1d_int_eltpos
	  (Geo.PolyFaceNb[poly] + 1, Geo.PolyFaceQty[poly], i) == -1)
      {
	if (verbosity) 
	  ut_print_message (2, 3, "poly %d in in poly list, but poly not based on face.\n",
	      poly);
	 
	return 2;
      }
  }

  return 0;
}

int
TestFaceState (struct GEO Geo, int i, int verbosity)
{
  int j, ver;

  /* if the face has state=0, it is not modified.
   * every of its vertices must be unmodified (state!=0).
   */
  if (Geo.FaceState[i] == 0)
  {
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
    {
      ver = Geo.FaceVerNb[i][j];
      if (Geo.VerState[ver] != 0)
      {
	if (verbosity) 
	  ut_print_message (2, 3, "has state = 0, but face ver (id = %d) has state = %d != 0.\n",
	      ver, Geo.VerState[ver]);

	return 1;
      }
    }

    return 0;
  }

  /* if the face has state>0, it is modified.
   * at least one of its vertices must be modified (state!=0).
   */
  else if (Geo.FaceState[i] > 0)
  {
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
    {
      ver = Geo.FaceVerNb[i][j];
      if (Geo.VerState[ver] != 0)
	return 0;
    }
	
    if (verbosity) 
      ut_print_message (2, 3, "state = %d, but all of the face vers have state = 0.\n",
	  Geo.FaceState[i]);

    return 1;
  }
  else
    return 0;			/* del face --> ok (maybe this is useless). */
}

/* TestFaceBound tests whether the face has 3 aligned vertices (on a domain edge)
 * or not. The case where three of the vertices of a face have 2 common bounds is
 * considered as critical.
 */
int
TestFaceBound (struct GEO Geo, int face, int verbosity)
{
  int i, ver, domver;

  int* bound1 = ut_alloc_1d_int (1);

  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    ver = Geo.FaceVerNb[face][i];
    if (Geo.VerDom[ver][0] == 1)
    {
      bound1[0]++;
      bound1 = ut_realloc_1d_int (bound1, bound1[0] + 1);
      bound1[bound1[0]] = Geo.VerDom[ver][1];
    }
    else if (Geo.VerDom[ver][0] == 0)
    {
      domver = Geo.VerDom[ver][1];
      bound1 = ut_realloc_1d_int (bound1, bound1[0] + 1 + Geo.DomVerEdgeQty[domver]);
      ut_array_1d_int_memcpy (bound1 + bound1[0] + 1, Geo.DomVerEdgeQty[domver],
	                                              Geo.DomVerEdgeNb[domver]);
      bound1[0] += Geo.DomVerEdgeQty[domver];
    }
  }

  int* bound2 = ut_alloc_1d_int (bound1[0] + 1);
  ut_array_1d_int_memcpy (bound2, bound1[0] + 1, bound1);
  ut_array_1d_int_sort_uniq (bound2 + 1, bound2[0], &(bound2[0]));

  for (i = 1; i <= bound2[0]; i++)
    if (ut_array_1d_int_nbofthisval (bound1 + 1, bound1[0], bound2[i]) == 3)
    {
      // 3 face vertices are on the same domain edge
      if (verbosity)
	ut_print_message (2, 3, "has 3 vertices lying on the same domain edge.\n");
      
      return 2;
    }

  ut_free_1d_int (bound1);
  ut_free_1d_int (bound2);

  return 0;
}

int
TestFaceSelfIntersect (struct GEO Geo, int face, int verbosity)
{
  int i;
  int res = 0;
  int cooqty = Geo.FaceVerQty[face];
  double** coo = ut_alloc_2d (cooqty, 3);
  double* n = ut_alloc_1d (3);
  double* sum_n = ut_alloc_1d (3);

  for (i = 0; i < Geo.FaceVerQty[face]; i++)
  {
    ut_array_1d_memcpy (coo[i], 3, Geo.VerCoo[Geo.FaceVerNb[face][i + 1]]);
    ut_space_projpoint_alongonto (coo[i], Geo.FaceEq[face] + 1, Geo.FaceEq[face]);
  }

  res = ut_space_contour_intersect (coo, cooqty);

  if (res > 0)
    if (verbosity)
      ut_print_message (2, 3, "has self-intersections (%d).\n", res);

  ut_free_2d (coo, Geo.FaceVerQty[face]);
  ut_free_1d (n);
  ut_free_1d (sum_n);

  return res;
}
  
int
neut_geo_test_face_normal (struct GEO Geo, int face, int verbosity)
{
  int i;
  int res = 0;
  int cooqty = Geo.FaceVerQty[face];
  double** coo = ut_alloc_2d (cooqty, 3);
  double* n = ut_alloc_1d (3);
  double* bary = ut_alloc_1d (3);
  double* sum_n = ut_alloc_1d (3);

  for (i = 0; i < Geo.FaceVerQty[face]; i++)
  {
    ut_array_1d_memcpy (coo[i], 3, Geo.VerCoo[Geo.FaceVerNb[face][i + 1]]);
    ut_array_1d_add (bary, coo[i], 3, bary);
  }
  ut_array_1d_scale (bary, 3, 1. / cooqty);

  for (i = 0; i < Geo.FaceVerQty[face]; i++)
  {
    ut_space_trianglenormal (bary, coo[i],
	coo[ut_num_rotpos (0, Geo.FaceVerQty[face] - 1, i, 1)], n);
    ut_array_1d_add (sum_n, n, 3, sum_n);
  }
  ut_array_1d_scale (sum_n, 3, 1. / ut_array_1d_norm (sum_n, 3));

  if (ut_vector_scalprod (Geo.FaceEq[face] + 1, sum_n) < 0)
  {
    if (verbosity)
      ut_print_message (2, 3, "normal is not properly oriented (scal = %f < 0).\n",
	  ut_vector_scalprod (Geo.FaceEq[face] + 1, sum_n));

    res = 1;
  }

  ut_free_2d (coo, Geo.FaceVerQty[face]);
  ut_free_1d (n);
  ut_free_1d (sum_n);
  ut_free_1d (bary);

  return res;
}
