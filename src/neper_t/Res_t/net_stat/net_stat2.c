/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat.h"

void
WriteStatGeoVer (FILE* file, char* format, struct GEO Geo)
{
  int i, j, t, b;
  char** val = NULL;
  int valqty;
  double* coo = ut_alloc_1d (3);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Geo.VerQty; i++)
  {
    ut_array_1d_memcpy (coo, 3, Geo.VerCoo[i]);
    for (j = 0; j < 3; j++)
      coo[j] = (fabs (Geo.VerCoo[i][j]) < 1e-12) ? 0 : Geo.VerCoo[i][j];

    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_geo_ver_true (Geo, Geo.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_geo_ver_polybodylevelmax (Geo, Geo.PolyBody, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", coo[0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", coo[1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", coo[2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", coo[0], coo[1], coo[2]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }
  }

  ut_free_2d_char (val, valqty);
  ut_free_1d (coo);

  return;
}

void
WriteStatGeoEdge (FILE* file, char* format, struct GEO Geo)
{
  int i, j;
  int t, b;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Geo.EdgeQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_geo_edge_true (Geo, Geo.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_geo_edge_polybodylevelmax (Geo, Geo.PolyBody, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", Geo.EdgeState[i]);
      else if (! strcmp (val[j], "length"))
	fprintf (file, "%.12lf", Geo.EdgeLength[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatGeoFace (FILE* file, char* format, struct GEO Geo)
{
  int i, j, t, b;
  double area;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Geo.FaceQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_geo_face_true (Geo, Geo.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_geo_face_polybodylevelmax (Geo, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "area"))
      {
	neut_geo_face_area (Geo, i, &area);
	fprintf (file, "%.12f", area);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", Geo.FaceState[i]);
      else if (! strcmp (val[j], "vernb"))
	fprintf (file, "%d", Geo.FaceVerQty[i]);
      else if (! strcmp (val[j], "ff"))
	fprintf (file, "%.12f", Geo.FaceFF[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatGeoPoly (FILE* file, char* format, struct GEO Geo)
{
  int i, j, k, l, s, face, neighpoly;
  double vol, area;
  char** val = NULL;
  int valqty;
  double* eq = ut_alloc_1d (4);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Geo.PolyQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", Geo.CenterCoo[i][0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", Geo.CenterCoo[i][1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", Geo.CenterCoo[i][2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", Geo.CenterCoo[i][0],
		       Geo.CenterCoo[i][1], Geo.CenterCoo[i][2]);
      else if (! strcmp (val[j], "xc"))
      {
	double* c = ut_alloc_1d (3);
	neut_geo_poly_centroid (Geo, i, c);
	fprintf (file, "%.12f", c[0]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "yc"))
      {
	double* c = ut_alloc_1d (3);
	neut_geo_poly_centroid (Geo, i, c);
	fprintf (file, "%.12f", c[1]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "zc"))
      {
	double* c = ut_alloc_1d (3);
	neut_geo_poly_centroid (Geo, i, c);
	fprintf (file, "%.12f", c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "pc"))
      {
	double* c = ut_alloc_1d (3);
	neut_geo_poly_centroid (Geo, i, c);
	fprintf (file, "%.12f %.12f %.12f", c[0], c[1], c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "true"))
	fprintf (file, "%d", Geo.PolyTrue[i]);
      else if (! strcmp (val[j], "body"))
	fprintf (file, "%d", Geo.PolyBody[i]);
      else if (! strcmp (val[j], "state"))
      {
	s = 0;
	for (k = 1; k <= Geo.PolyFaceQty[i]; k++)
	  if (Geo.FaceState[Geo.PolyFaceNb[i][k]] != 0)
	  {
	    s = 1;
	    break;
	  }
	fprintf (file, "%d", s);
      }
      else if (! strcmp (val[j], "vol"))
      {
	neut_geo_poly_volume (Geo, i, &vol);
	fprintf (file, "%.12f", vol);
      }
      else if (! strcmp (val[j], "vernb"))
      {
	int* list = NULL;
	int verqty;
	neut_geo_poly_vers  (Geo, i, &list, &verqty);
	fprintf (file, "%d", verqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "verlist"))
      {
	int* list = NULL;
	int verqty;
	neut_geo_poly_vers  (Geo, i, &list, &verqty);
	for (k = 0; k < verqty; k++)
	  fprintf (file, "%d%s", list[k], (k < verqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgenb"))
      {
	int* list = NULL;
	int edgeqty;
	neut_geo_poly_edges (Geo, i, &list, &edgeqty);
	fprintf (file, "%d", edgeqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgelist"))
      {
	int* list = NULL;
	int edgeqty;
	neut_geo_poly_edges (Geo, i, &list, &edgeqty);
	for (k = 0; k < edgeqty; k++)
	  fprintf (file, "%d%s", list[k], (k < edgeqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "facenb"))
	fprintf (file, "%d", Geo.PolyFaceQty[i]);
      else if (! strcmp (val[j], "facelist"))
      {
	for (k = 1; k <= Geo.PolyFaceQty[i]; k++)
	  fprintf (file, "%d%s", Geo.PolyFaceNb[i][k], (k < Geo.PolyFaceQty[i]) ? " " : "");
      }
      else if (! strcmp (val[j], "npolylist"))
      {
	for (k = 1; k <= Geo.PolyFaceQty[i]; k++)
	{
	  face = Geo.PolyFaceNb[i][k]; 
	  neighpoly = (Geo.FacePoly[face][0] != i) ?
		       Geo.FacePoly[face][0] : Geo.FacePoly[face][1];
	  fprintf (file, "%d%s", neighpoly,
	      (k < Geo.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "facearealist"))
      {
	for (k = 1; k <= Geo.PolyFaceQty[i]; k++)
	{
	  face = Geo.PolyFaceNb[i][k];
	  neut_geo_face_area (Geo, face, &area);
	  fprintf (file, "%.12f%s", area,
	      (k < Geo.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "faceeqlist"))
      {
	for (k = 1; k <= Geo.PolyFaceQty[i]; k++)
	{
	  face = Geo.PolyFaceNb[i][k];
	  ut_array_1d_memcpy (eq, 4, Geo.FaceEq[face]);
	  if (Geo.PolyFaceOri[i][k] == -1)
	    ut_array_1d_scale (eq, 4, -1);
	  for (l = 0; l < 4; l++)
	    fprintf (file, "%.12f%s", eq[l],
		! (l == 3 && k == Geo.PolyFaceQty[i]) ? " " : "");
	}
      }

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);
  ut_free_1d (eq);

  return;
}
