/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_geo_fprintf_lcl.h"

void
neut_geo_fprintf (FILE* file, struct GEO Geo)
{
  GEHead   (Geo, file);
  GEVer    (Geo, file);
  GEEdge   (Geo, file);
  GEFace   (Geo, file);
  GEPoly   (Geo, file);
  GEDomain (Geo, file);
  GEFoot   (file);

  return;
}

void
neut_geo_fprintf_gmsh (FILE* file, struct GEO Geo)
{
  int i, j;

  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
      fprintf (file, "Point (%d) = {%.12f,%.12f,%.12f};\n", 
	  i, Geo.VerCoo[i][0], Geo.VerCoo[i][1], Geo.VerCoo[i][2]);

  for (i = 1; i <= Geo.EdgeQty; i++)
    if (Geo.EdgeState[i] != -1)
      fprintf (file, "Line (%d) = {%d,%d};\n", 
	  i, Geo.EdgeVerNb[i][0], Geo.EdgeVerNb[i][1]);

  for (i = 1; i <= Geo.FaceQty; i++)
    if (Geo.FaceState[i] != -1)
    {
      fprintf (file, "Line Loop (%d) = {", i);
      for (j = 1; j <= Geo.FaceVerQty[i]; j++)
      {
	fprintf (file, "%d%s", Geo.FaceEdgeNb[i][j] * Geo.FaceEdgeOri[i][j],
	    (j < Geo.FaceVerQty[i]) ? "," : "");
      }
      fprintf (file, "};\n");
      fprintf (file, "Plane Surface (%d) = {%d}; ", i, i);
      fprintf (file, "Physical Surface (%d) = {%d};\n", i, i);
    }

  for (i = 1; i <= Geo.PolyQty; i++)
  {
    fprintf (file, " Surface Loop (%d) = {", i);
    for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
    {
      fprintf (file, "%d%s", Geo.PolyFaceNb[i][j] * Geo.PolyFaceOri[i][j],
	  (j < Geo.PolyFaceQty[i]) ? "," : "");
    }
    fprintf (file, "};\n");
    fprintf (file, "Volume (%d) = {%d}; ", i, i);
  }

  return;
}

void
neut_geo_fprintf_ply (FILE* file, struct GEO Geo)
{
  int i, j;

  fprintf (file, "ply\n");
  fprintf (file, "format ascii 1.0\n");
  fprintf (file, "element vertex %d\n", Geo.VerQty);
  fprintf (file, "property float x\n");
  fprintf (file, "property float y\n");
  fprintf (file, "property float z\n");
  fprintf (file, "element face %d\n", Geo.FaceQty);
  fprintf (file, "property list uchar int vertex_indices\n");
  fprintf (file, "element cell %d\n", Geo.PolyQty);
  fprintf (file, "property list uchar int face_indices\n");
  fprintf (file, "end_header\n");

  for (i = 1; i <= Geo.VerQty; i++)
    if (Geo.VerState[i] != -1)
      fprintf (file, "%.12f %.12f %.12f\n", 
	  Geo.VerCoo[i][0], Geo.VerCoo[i][1], Geo.VerCoo[i][2]);

  for (i = 1; i <= Geo.FaceQty; i++)
  {
    fprintf (file, "%d ", Geo.FaceVerQty[i]);
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
      fprintf (file, "%d%s", Geo.FaceVerNb[i][j] - 1,
		(j < Geo.FaceVerQty[i]) ? " " : "\n");
  }
  
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    fprintf (file, "%d ", Geo.PolyFaceQty[i]);
    for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
      fprintf (file, "%d%s", Geo.PolyFaceNb[i][j] - 1,
	  (j < Geo.PolyFaceQty[i]) ? " " : "\n");
  }

  return;
}
