/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat.h"

void
WriteStatTessVer (FILE* file, char* format, struct TESS Tess)
{
  int i, j, t, b;
  char** val = NULL;
  int valqty;
  double* coo = ut_alloc_1d (3);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Tess.VerQty; i++)
  {
    ut_array_1d_memcpy (coo, 3, Tess.VerCoo[i]);
    for (j = 0; j < 3; j++)
      coo[j] = (fabs (Tess.VerCoo[i][j]) < 1e-12) ? 0 : Tess.VerCoo[i][j];

    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_ver_true (Tess, Tess.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_ver_polybodylevelmax (Tess, Tess.PolyBody, i);
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
WriteStatTessEdge (FILE* file, char* format, struct TESS Tess)
{
  int i, j;
  int t, b;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Tess.EdgeQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_edge_true (Tess, Tess.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_edge_polybodylevelmax (Tess, Tess.PolyBody, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", Tess.EdgeState[i]);
      else if (! strcmp (val[j], "length"))
	fprintf (file, "%.12lf", Tess.EdgeLength[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatTessFace (FILE* file, char* format, struct TESS Tess)
{
  int i, j, t, b;
  double area;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Tess.FaceQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_face_true (Tess, Tess.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_face_polybodylevelmax (Tess, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "area"))
      {
	neut_tess_face_area (Tess, i, &area);
	fprintf (file, "%.12f", area);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", Tess.FaceState[i]);
      else if (! strcmp (val[j], "vernb"))
	fprintf (file, "%d", Tess.FaceVerQty[i]);
      else if (! strcmp (val[j], "ff"))
	fprintf (file, "%.12f", Tess.FaceFF[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatTessPoly (FILE* file, char* format, struct TESS Tess)
{
  int i, j, k, l, s, face, neighpoly;
  double vol, area;
  char** val = NULL;
  int valqty;
  double* eq = ut_alloc_1d (4);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= Tess.PolyQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", Tess.CenterCoo[i][0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", Tess.CenterCoo[i][1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", Tess.CenterCoo[i][2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", Tess.CenterCoo[i][0],
		       Tess.CenterCoo[i][1], Tess.CenterCoo[i][2]);
      else if (! strcmp (val[j], "xc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (Tess, i, c);
	fprintf (file, "%.12f", c[0]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "yc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (Tess, i, c);
	fprintf (file, "%.12f", c[1]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "zc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (Tess, i, c);
	fprintf (file, "%.12f", c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "pc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (Tess, i, c);
	fprintf (file, "%.12f %.12f %.12f", c[0], c[1], c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "true"))
	fprintf (file, "%d", Tess.PolyTrue[i]);
      else if (! strcmp (val[j], "body"))
	fprintf (file, "%d", Tess.PolyBody[i]);
      else if (! strcmp (val[j], "state"))
      {
	s = 0;
	for (k = 1; k <= Tess.PolyFaceQty[i]; k++)
	  if (Tess.FaceState[Tess.PolyFaceNb[i][k]] != 0)
	  {
	    s = 1;
	    break;
	  }
	fprintf (file, "%d", s);
      }
      else if (! strcmp (val[j], "vol"))
      {
	neut_tess_poly_volume (Tess, i, &vol);
	fprintf (file, "%.12f", vol);
      }
      else if (! strcmp (val[j], "vernb"))
      {
	int* list = NULL;
	int verqty;
	neut_tess_poly_vers  (Tess, i, &list, &verqty);
	fprintf (file, "%d", verqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "verlist"))
      {
	int* list = NULL;
	int verqty;
	neut_tess_poly_vers  (Tess, i, &list, &verqty);
	for (k = 0; k < verqty; k++)
	  fprintf (file, "%d%s", list[k], (k < verqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgenb"))
      {
	int* list = NULL;
	int edgeqty;
	neut_tess_poly_edges (Tess, i, &list, &edgeqty);
	fprintf (file, "%d", edgeqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgelist"))
      {
	int* list = NULL;
	int edgeqty;
	neut_tess_poly_edges (Tess, i, &list, &edgeqty);
	for (k = 0; k < edgeqty; k++)
	  fprintf (file, "%d%s", list[k], (k < edgeqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "facenb"))
	fprintf (file, "%d", Tess.PolyFaceQty[i]);
      else if (! strcmp (val[j], "facelist"))
      {
	for (k = 1; k <= Tess.PolyFaceQty[i]; k++)
	  fprintf (file, "%d%s", Tess.PolyFaceNb[i][k], (k < Tess.PolyFaceQty[i]) ? " " : "");
      }
      else if (! strcmp (val[j], "npolylist"))
      {
	for (k = 1; k <= Tess.PolyFaceQty[i]; k++)
	{
	  face = Tess.PolyFaceNb[i][k]; 
	  neighpoly = (Tess.FacePoly[face][0] != i) ?
		       Tess.FacePoly[face][0] : Tess.FacePoly[face][1];
	  fprintf (file, "%d%s", neighpoly,
	      (k < Tess.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "facearealist"))
      {
	for (k = 1; k <= Tess.PolyFaceQty[i]; k++)
	{
	  face = Tess.PolyFaceNb[i][k];
	  neut_tess_face_area (Tess, face, &area);
	  fprintf (file, "%.12f%s", area,
	      (k < Tess.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "faceeqlist"))
      {
	for (k = 1; k <= Tess.PolyFaceQty[i]; k++)
	{
	  face = Tess.PolyFaceNb[i][k];
	  ut_array_1d_memcpy (eq, 4, Tess.FaceEq[face]);
	  if (Tess.PolyFaceOri[i][k] == -1)
	    ut_array_1d_scale (eq, 4, -1);
	  for (l = 0; l < 4; l++)
	    fprintf (file, "%.12f%s", eq[l],
		! (l == 3 && k == Tess.PolyFaceQty[i]) ? " " : "");
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
