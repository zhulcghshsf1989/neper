/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "GeoStuff.h"

void
WriteStatGeoPoly (FILE* file, struct GEO Geo)
{
  int i, j, polystate, verqty, edgeqty;
  int* list = NULL;
  double vol;
  double* c = ut_alloc_1d (3);

  for (i = 1; i <= Geo.PolyQty; i++)
  {
    polystate = 0;
    for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
      if (Geo.FaceState[Geo.PolyFaceNb[i][j]] != 0)
      {
	polystate = 1;
	break;
      }

    neut_geo_poly_vers  (Geo, i, &list, &verqty);
    neut_geo_poly_edges (Geo, i, &list, &edgeqty);

    neut_geo_poly_volume (Geo, i, &vol);
    neut_geo_poly_centroid (Geo, i, c);
    fprintf (file, "%d %d %d %d %.12f %.12f %.12f %d %d %d %.12f %.12f %.12f %.12f\n", i,
	  Geo.PolyTrue[i], Geo.PolyBody[i], polystate, 
	  Geo.CenterCoo[i][0], Geo.CenterCoo[i][1], Geo.CenterCoo[i][2],
	  verqty, edgeqty, Geo.PolyFaceQty[i], vol, c[0], c[1], c[2]);
  }

  ut_free_1d_int (list);
  ut_free_1d (c);

  return;
}

void
WriteStatGeoFace (FILE* file, struct GEO Geo)
{
  int i, t, b;
  double area;

  for (i = 1; i <= Geo.FaceQty; i++)
  {
    t = neut_geo_face_true (Geo, Geo.PolyTrue, i);
    b = neut_geo_face_polybodylevelmax (Geo, i);

    neut_geo_face_area (Geo, i, &area);

    fprintf (file, "%d %d %d %d %d %.12e %10.6f\n", i, t, b, Geo.FaceState[i], Geo.FaceVerQty[i], area, Geo.FaceFF[i]);
  }

  return;
}

void
WriteStatGeoEdge (FILE* out, struct GEO Geo)
{
  int i, t, b;

  for (i = 1; i <= Geo.EdgeQty; i++)
  {
    t = neut_geo_edge_true (Geo, Geo.PolyTrue, i);
    b = neut_geo_edge_polybodylevelmax (Geo, Geo.PolyBody, i);

    fprintf (out, "%d %d %d %d %.12e\n", i, t, b, Geo.EdgeState[i], Geo.EdgeLength[i]);
  }

  return;
}

/*
void
WriteStatGeoFaceAngle (FILE* out, struct GEO Geo)
{
  int i, t = 0, j;
  int ver;
  double *vect1 = ut_alloc_1d (3);
  double *vect2 = ut_alloc_1d (3);
  double angle;

  for (i = 1; i <= Geo.FaceQty; i++)
  {
    if (Geo.FaceState[i] >= 0)
      t = neut_geo_face_true (Geo, Geo.PolyTrue, i);
      
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
    {
      ver = Geo.FaceVerNb[i][j];
      AngleAtVer (Geo, i, ver, &angle);

      fprintf (out, "%d-%d %d %d %.10f\n", i, j, Geo.FaceState[i], t, angle);
    }
  }

  ut_free_1d (vect1);
  ut_free_1d (vect2);

  return;
}
*/

void
WriteStatGeoVer (FILE* out, struct GEO Geo)
{
  int i;
  int true_, body;

  for (i = 1; i <= Geo.VerQty; i++)
  {
    true_ = neut_geo_ver_true (Geo, Geo.PolyTrue, i);
    body  = neut_geo_ver_polybodylevelmax (Geo, Geo.PolyBody, i);
    fprintf (out, "%d %d %d %d %.12f %.12f %.12f\n", i, true_, body, 0,
	Geo.VerCoo[i][0], Geo.VerCoo[i][1], Geo.VerCoo[i][2]);

  }

  return;
}
