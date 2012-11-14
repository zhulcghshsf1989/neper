/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestFace.h"

int
TestFaceReciprocityEdge (struct GEO Geo, int i)
{
  int j;
  int edge;

  if (Geo.FaceVerQty[i] < 3)
  {
    /*
       ut_print_lineheader(2);
       printf("Face %d has less than 3 = %d vers/edges!\n",i,Geo.FaceVerQty[i]);
     */
    return 2;
  }

  for (j = 1; j <= Geo.FaceVerQty[i]; j++)
  {
    edge = Geo.FaceEdgeNb[i][j];
    if (ut_array_1d_int_eltpos
	(Geo.EdgeFaceNb[edge], Geo.EdgeFaceQty[edge], i) == -1)
    {
      /*
         ut_print_lineheader(2);
         printf("edge %d of face %d = edge %d has not face %d!\n",j,i,edge,i);
       */
      return 2;
    }
  }

  return 0;
}

int
TestFaceReciprocityVer (struct GEO Geo, int i)
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
      /*
         ut_print_lineheader(2);
         printf("ver %d of face %d = ver %d has not face %d!\n",j,i,ver,i);
       */
      return 2;
    }
  }

  ut_free_1d_int (face);

  return 0;
}

int
TestFaceReciprocityPoly (struct GEO Geo, int i)
{
  int j;
  int poly;

  if (Geo.FacePoly[i][0] < 0 && Geo.FacePoly[i][1] < 0)
  {
    /*
     ut_print_lineheader(2);
     printf("face %d has 2 non-real poly!\n",i);
     */
    return 2;
  }

  for (j = 0; j < 2; j++)
  {
    poly = Geo.FacePoly[i][j];
    if (poly > 0)
      if (ut_array_1d_int_eltpos
	  (Geo.PolyFaceNb[poly] + 1, Geo.PolyFaceQty[poly], i) == -1)
      {
	/*
	   ut_print_lineheader(2);
	   printf("poly %d of face %d = poly %d has not face %d!\n",j,i,poly,i);
	 */
	return 2;
      }
  }

  return 0;
}

/* TestFaceState checks FaceState. If the face is set as modified, at
 * least one of its vertices must be modified. If the face is set as
 * unmodified, all its vertices must be unmodified.
 */
int
TestFaceState (struct GEO Geo, int i)
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
	return 1;		/* modified --> pb */
    }
    return 0;			/* unmodified --> ok */
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
	return 0;		/* one ver modified --> ok */
    }
    return 1;			/* no ver modified --> pb */
  }
  else
    return 0;			/* del face --> ok (maybe this is useless). */
}

/* TestFaceBound tests whether the face has 3 aligned vertices (on a domain edge)
 * or not. The case where three of the vertices of a face have 2 common bounds is
 * considered as critical.
 */
int
TestFaceBound (struct GEO Geo, int face)
{
  int i, ver, domver, status = 0;
  int verbosity = 0;

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
      if (verbosity > 0)
      {
	printf ("bound1 = ");
	ut_array_1d_int_fprintf (stdout, bound1, bound1[0] + 1, "%d");
	printf ("bound2 = ");
	ut_array_1d_int_fprintf (stdout, bound2, bound2[0] + 1, "%d");
      }
      status = 2;
      break;
    }

  ut_free_1d_int (bound1);
  ut_free_1d_int (bound2);

  return status;
}

int
TestFaceSelfIntersect (struct GEO Geo, int face)
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
  // ut_array_2d_fprintf (stdout, coo, cooqty, 3, "%.15f");

  ut_free_2d (coo, Geo.FaceVerQty[face]);
  ut_free_1d (n);
  ut_free_1d (sum_n);

  return res;
}
  
int
neut_geo_test_face_normal (struct GEO Geo, int face)
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
    res = 1;

  ut_free_2d (coo, Geo.FaceVerQty[face]);
  ut_free_1d (n);
  ut_free_1d (sum_n);
  ut_free_1d (bary);

  return res;
}
