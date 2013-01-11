/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_stat.h"

void
WriteStatTessVer (FILE* file, char* format, struct TESS net_poly_tesl)
{
  int i, j, t, b;
  char** val = NULL;
  int valqty;
  double* coo = ut_alloc_1d (3);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= net_poly_tesl.VerQty; i++)
  {
    ut_array_1d_memcpy (coo, 3, net_poly_tesl.VerCoo[i]);
    for (j = 0; j < 3; j++)
      coo[j] = (fabs (net_poly_tesl.VerCoo[i][j]) < 1e-12) ? 0 : net_poly_tesl.VerCoo[i][j];

    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_ver_true (net_poly_tesl, net_poly_tesl.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_ver_polybodylevelmax (net_poly_tesl, net_poly_tesl.PolyBody, i);
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
WriteStatTessEdge (FILE* file, char* format, struct TESS net_poly_tesl)
{
  int i, j;
  int t, b;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= net_poly_tesl.EdgeQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_edge_true (net_poly_tesl, net_poly_tesl.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_edge_polybodylevelmax (net_poly_tesl, net_poly_tesl.PolyBody, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", net_poly_tesl.EdgeState[i]);
      else if (! strcmp (val[j], "length"))
	fprintf (file, "%.12lf", net_poly_tesl.EdgeLength[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatTessFace (FILE* file, char* format, struct TESS net_poly_tesl)
{
  int i, j, t, b;
  double area;
  char** val = NULL;
  int valqty;

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= net_poly_tesl.FaceQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "true"))
      {
	t = neut_tess_face_true (net_poly_tesl, net_poly_tesl.PolyTrue, i);
	fprintf (file, "%d", t);
      }
      else if (! strcmp (val[j], "body"))
      {
	b = neut_tess_face_polybodylevelmax (net_poly_tesl, i);
	fprintf (file, "%d", b);
      }
      else if (! strcmp (val[j], "area"))
      {
	neut_tess_face_area (net_poly_tesl, i, &area);
	fprintf (file, "%.12f", area);
      }
      else if (! strcmp (val[j], "state"))
	fprintf (file, "%d", net_poly_tesl.FaceState[i]);
      else if (! strcmp (val[j], "vernb"))
	fprintf (file, "%d", net_poly_tesl.FaceVerQty[i]);
      else if (! strcmp (val[j], "ff"))
	fprintf (file, "%.12f", net_poly_tesl.FaceFF[i]);

      if (j < valqty - 1)
	fprintf (file, " ");
      else
	fprintf (file, "\n");
    }

  ut_free_2d_char (val, valqty);

  return;
}

void
WriteStatTessPoly (FILE* file, char* format, struct TESS net_poly_tesl)
{
  int i, j, k, l, s, face, neighpoly;
  double vol, area;
  char** val = NULL;
  int valqty;
  double* eq = ut_alloc_1d (4);

  ut_string_separate (format, ',', &val, &valqty);

  for (i = 1; i <= net_poly_tesl.PolyQty; i++)
    for (j = 0; j < valqty; j++)
    {
      if (! strcmp (val[j], "id"))
	fprintf (file, "%d", i);
      else if (! strcmp (val[j], "x"))
	fprintf (file, "%.12f", net_poly_tesl.CenterCoo[i][0]);
      else if (! strcmp (val[j], "y"))
	fprintf (file, "%.12f", net_poly_tesl.CenterCoo[i][1]);
      else if (! strcmp (val[j], "z"))
	fprintf (file, "%.12f", net_poly_tesl.CenterCoo[i][2]);
      else if (! strcmp (val[j], "p"))
	fprintf (file, "%.12f %.12f %.12f", net_poly_tesl.CenterCoo[i][0],
		       net_poly_tesl.CenterCoo[i][1], net_poly_tesl.CenterCoo[i][2]);
      else if (! strcmp (val[j], "xc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (net_poly_tesl, i, c);
	fprintf (file, "%.12f", c[0]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "yc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (net_poly_tesl, i, c);
	fprintf (file, "%.12f", c[1]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "zc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (net_poly_tesl, i, c);
	fprintf (file, "%.12f", c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "pc"))
      {
	double* c = ut_alloc_1d (3);
	neut_tess_poly_centroid (net_poly_tesl, i, c);
	fprintf (file, "%.12f %.12f %.12f", c[0], c[1], c[2]);
	ut_free_1d (c);
      }
      else if (! strcmp (val[j], "true"))
	fprintf (file, "%d", net_poly_tesl.PolyTrue[i]);
      else if (! strcmp (val[j], "body"))
	fprintf (file, "%d", net_poly_tesl.PolyBody[i]);
      else if (! strcmp (val[j], "state"))
      {
	s = 0;
	for (k = 1; k <= net_poly_tesl.PolyFaceQty[i]; k++)
	  if (net_poly_tesl.FaceState[net_poly_tesl.PolyFaceNb[i][k]] != 0)
	  {
	    s = 1;
	    break;
	  }
	fprintf (file, "%d", s);
      }
      else if (! strcmp (val[j], "vol"))
      {
	neut_tess_poly_volume (net_poly_tesl, i, &vol);
	fprintf (file, "%.12f", vol);
      }
      else if (! strcmp (val[j], "vernb"))
      {
	int* list = NULL;
	int verqty;
	neut_tess_poly_vers  (net_poly_tesl, i, &list, &verqty);
	fprintf (file, "%d", verqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "verlist"))
      {
	int* list = NULL;
	int verqty;
	neut_tess_poly_vers  (net_poly_tesl, i, &list, &verqty);
	for (k = 0; k < verqty; k++)
	  fprintf (file, "%d%s", list[k], (k < verqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgenb"))
      {
	int* list = NULL;
	int edgeqty;
	neut_tess_poly_edges (net_poly_tesl, i, &list, &edgeqty);
	fprintf (file, "%d", edgeqty);
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "edgelist"))
      {
	int* list = NULL;
	int edgeqty;
	neut_tess_poly_edges (net_poly_tesl, i, &list, &edgeqty);
	for (k = 0; k < edgeqty; k++)
	  fprintf (file, "%d%s", list[k], (k < edgeqty - 1) ? " " : "");
	ut_free_1d_int (list);
      }
      else if (! strcmp (val[j], "facenb"))
	fprintf (file, "%d", net_poly_tesl.PolyFaceQty[i]);
      else if (! strcmp (val[j], "facelist"))
      {
	for (k = 1; k <= net_poly_tesl.PolyFaceQty[i]; k++)
	  fprintf (file, "%d%s", net_poly_tesl.PolyFaceNb[i][k], (k < net_poly_tesl.PolyFaceQty[i]) ? " " : "");
      }
      else if (! strcmp (val[j], "npolylist"))
      {
	for (k = 1; k <= net_poly_tesl.PolyFaceQty[i]; k++)
	{
	  face = net_poly_tesl.PolyFaceNb[i][k]; 
	  neighpoly = (net_poly_tesl.FacePoly[face][0] != i) ?
		       net_poly_tesl.FacePoly[face][0] : net_poly_tesl.FacePoly[face][1];
	  fprintf (file, "%d%s", neighpoly,
	      (k < net_poly_tesl.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "facearealist"))
      {
	for (k = 1; k <= net_poly_tesl.PolyFaceQty[i]; k++)
	{
	  face = net_poly_tesl.PolyFaceNb[i][k];
	  neut_tess_face_area (net_poly_tesl, face, &area);
	  fprintf (file, "%.12f%s", area,
	      (k < net_poly_tesl.PolyFaceQty[i]) ? " " : "");
	}
      }
      else if (! strcmp (val[j], "faceeqlist"))
      {
	for (k = 1; k <= net_poly_tesl.PolyFaceQty[i]; k++)
	{
	  face = net_poly_tesl.PolyFaceNb[i][k];
	  ut_array_1d_memcpy (eq, 4, net_poly_tesl.FaceEq[face]);
	  if (net_poly_tesl.PolyFaceOri[i][k] == -1)
	    ut_array_1d_scale (eq, 4, -1);
	  for (l = 0; l < 4; l++)
	    fprintf (file, "%.12f%s", eq[l],
		! (l == 3 && k == net_poly_tesl.PolyFaceQty[i]) ? " " : "");
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
