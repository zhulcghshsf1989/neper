/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestPoly.h"

int
TestPolyReciprocityFace (struct GEO Geo, int i, int verbosity)
{
  int j;
  int face;

  if (Geo.PolyFaceQty[i] < 4)
  {
    if (verbosity)
      ut_print_message (2, 3, "number of faces = %d < 4.\n", Geo.PolyFaceQty[i]);

    return 2;
  }

  for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
  {
    face = Geo.PolyFaceNb[i][j];

    if (ut_array_1d_int_eltpos (Geo.FacePoly[face], 2, i) == -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "based on face %d, but poly is not in face poly list.\n", face);
       
      return 3;
    }
  }

  return 0;
}

/* this routines tests if several faces belongs to the same domain
 * boundary, which means that there are coplanar. */
int
TestPolyCoplaneityFace (struct GEO Geo, int i, int verbosity)
{
  int j, face, res = 0;
  int *tmp = ut_alloc_1d_int (Geo.PolyFaceQty[i] + 1);

  tmp[0] = Geo.PolyFaceQty[i];
  for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
  {
    face = Geo.PolyFaceNb[i][j];
    tmp[j] = (Geo.FaceDom[face][0] == 2) ? Geo.FaceDom[face][1] : -1;
  }

  for (j = 1; j <= Geo.DomFaceQty; j++)
    if (ut_array_1d_int_nbofthisval (tmp + 1, tmp[0], j) > 1)
    {
      if (verbosity)
	ut_print_message (2, 3, "has several faces which belong to the same domain face.\n");

      res = 3;
      break;
    }

  ut_free_1d_int (tmp);

  return res;
}

int
TestPolyOrientationFace (struct GEO Geo, int i, int verbosity)
{
  int j, face;
  double* eq = ut_alloc_1d (4);

  for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
  {
    face = Geo.PolyFaceNb[i][j];

    ut_array_1d_memcpy (eq, 4, Geo.FaceEq[face]);
    ut_array_1d_scale (eq, 4, Geo.PolyFaceOri[i][j]);

    if (ut_space_planeside (eq, Geo.CenterCoo[i] - 1) != -1)
    {
      if (verbosity)
	ut_print_message (2, 3, "face %d is not properly oriented.\n", face);

      return 1;
    }
  }

  ut_free_1d (eq);

  return 0;
}

/*
int
TestPolyOrientationFace (struct GEO Geo, int i, int verbosity)
{
  int j, k, face, res = 0;
  int* edge_faces = NULL;
  int edgeqty;
  int* edge = NULL;
  int* face_ori = ut_alloc_1d_int (2);
  int* edge_ori = ut_alloc_1d_int (2);

  neut_geo_poly_edges (Geo, i, &edge, &edgeqty);

  for (j = 0; j < edgeqty; j++)
  {
    neut_geo_poly_edge_faces (Geo, i, edge[j], &edge_faces);

    printf ("j = %d edge = %d\n", j, edge[j]);

    printf ("face = ");
    for (k = 0; k < 2; k++)
    {
      face = edge_faces[k];
      printf ("%d ", face);
      neut_geo_poly_face_ori (Geo, i, face, &(face_ori[k]));
      neut_geo_face_edge_ori (Geo, face, edge[j], &(edge_ori[k]));
    }
    printf ("\n");

    printf ("edge_ori = %d %d face_ori = %d %d\n", edge_ori[0], edge_ori[1], face_ori[0], face_ori[1]);

    if (edge_ori[0] * edge_ori[1] * face_ori[0] * face_ori[1] != -1)
    {
      res = 1;
      break;
    }
  }

  ut_free_1d_int (edge_faces);
  ut_free_1d_int (edge);
  ut_free_1d_int (face_ori);
  ut_free_1d_int (edge_ori);

  return res;
}
*/
