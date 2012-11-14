/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestPoly.h"

int
TestPolyReciprocityFace (struct GEO Geo, int i)
{
  int j;
  int face;

  if (Geo.PolyFaceQty[i] < 4)
  {
    ut_print_messagewnc (2, 72,
		"A polyhedron has a wrong amount of faces.  Classically, this is due to a (too) high value of critical length for the small edges.  You should decrease rcl and/or increase pcl (or decrease sel if you are using it).\n");

    abort ();
    return 3;
  }

  for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
  {
    face = Geo.PolyFaceNb[i][j];
    if (ut_array_1d_int_eltpos (Geo.FacePoly[face], 2, i) == -1)
    {
      /*
         ut_print_lineheader(2);
         printf("face %d of poly %d = face %d has not poly %d!\n",j,i,face,i);
       */
      return 3;
    }
  }

  return 0;
}

/* this routines tests if several faces belongs to the same domain
 * boundary, which means that there are coplanar. */
int
TestPolyCoplaneityFace (struct GEO Geo, int i)
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
      res = 3;
      break;
    }

  ut_free_1d_int (tmp);

  return res;
}

int
TestPolyOrientationFace (struct GEO Geo, int i)
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

    for (k = 0; k < 2; k++)
    {
      face = edge_faces[k];
      neut_geo_poly_face_ori (Geo, i, face, &(face_ori[k]));
      neut_geo_face_edge_ori (Geo, face, edge[j], &(edge_ori[k]));

      // printf ("k = %d face = %d\n", k, face);
    }
    if (edge_ori[0] * edge_ori[1] * face_ori[0] * face_ori[1] != -1)
    {
      // printf ("edge_ori = %d %d face_ori = %d %d\n", edge_ori[0], edge_ori[1], face_ori[0], face_ori[1]);
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
