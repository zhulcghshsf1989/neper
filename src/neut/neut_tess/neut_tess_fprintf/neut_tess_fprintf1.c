/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_fprintf_lcl.h"

void
neut_tess_fprintf (FILE* file, struct TESS Tess)
{
  GEHead   (Tess, file);
  GEVer    (Tess, file);
  GEEdge   (Tess, file);
  GEFace   (Tess, file);
  GEPoly   (Tess, file);
  GEDomain (Tess, file);
  GEFoot   (file);

  return;
}

void
neut_tess_fprintf_gmsh (FILE* file, struct TESS Tess)
{
  int i, j;

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      fprintf (file, "Point (%d) = {%.12f,%.12f,%.12f};\n", 
	  i, Tess.VerCoo[i][0], Tess.VerCoo[i][1], Tess.VerCoo[i][2]);

  for (i = 1; i <= Tess.EdgeQty; i++)
    if (Tess.EdgeState[i] != -1)
      fprintf (file, "Line (%d) = {%d,%d};\n", 
	  i, Tess.EdgeVerNb[i][0], Tess.EdgeVerNb[i][1]);

  for (i = 1; i <= Tess.FaceQty; i++)
    if (Tess.FaceState[i] != -1)
    {
      fprintf (file, "Line Loop (%d) = {", i);
      for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      {
	fprintf (file, "%d%s", Tess.FaceEdgeNb[i][j] * Tess.FaceEdgeOri[i][j],
	    (j < Tess.FaceVerQty[i]) ? "," : "");
      }
      fprintf (file, "};\n");
      fprintf (file, "Plane Surface (%d) = {%d}; ", i, i);
      fprintf (file, "Physical Surface (%d) = {%d};\n", i, i);
    }

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, " Surface Loop (%d) = {", i);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      fprintf (file, "%d%s", Tess.PolyFaceNb[i][j] * Tess.PolyFaceOri[i][j],
	  (j < Tess.PolyFaceQty[i]) ? "," : "");
    }
    fprintf (file, "};\n");
    fprintf (file, "Volume (%d) = {%d}; ", i, i);
  }

  return;
}

void
neut_tess_fprintf_ply (FILE* file, struct TESS Tess)
{
  int i, j;

  fprintf (file, "ply\n");
  fprintf (file, "format ascii 1.0\n");
  fprintf (file, "element vertex %d\n", Tess.VerQty);
  fprintf (file, "property float x\n");
  fprintf (file, "property float y\n");
  fprintf (file, "property float z\n");
  fprintf (file, "element face %d\n", Tess.FaceQty);
  fprintf (file, "property list uchar int vertex_indices\n");
  fprintf (file, "element cell %d\n", Tess.PolyQty);
  fprintf (file, "property list uchar int face_indices\n");
  fprintf (file, "end_header\n");

  for (i = 1; i <= Tess.VerQty; i++)
    if (Tess.VerState[i] != -1)
      fprintf (file, "%.12f %.12f %.12f\n", 
	  Tess.VerCoo[i][0], Tess.VerCoo[i][1], Tess.VerCoo[i][2]);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, "%d ", Tess.FaceVerQty[i]);
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, "%d%s", Tess.FaceVerNb[i][j] - 1,
		(j < Tess.FaceVerQty[i]) ? " " : "\n");
  }
  
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "%d ", Tess.PolyFaceQty[i]);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, "%d%s", Tess.PolyFaceNb[i][j] - 1,
	  (j < Tess.PolyFaceQty[i]) ? " " : "\n");
  }

  return;
}

void
neut_tess_fprintf_dec (FILE* file, struct TESS Tess)
{
  int i, j, k, l, face, ver, faceid;

  fprintf (file, "; 3DEC input deck produced by Neper.\n");

  faceid = 0;
  for (i = 1; i <= Tess.PolyQty; i++)
  {
    fprintf (file, "; block %d\n", i);
    fprintf (file, "poly reg %4d mat   1 con   1 &\n", i);

    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
    {
      face = Tess.PolyFaceNb[i][j];

      fprintf (file, "face ID %9d ", ++faceid);
      for (k = 1; k <= Tess.FaceVerQty[face]; k++)
      {
	ver = Tess.FaceVerNb[face][k];
	if (k > 1)
	  fprintf (file, "                  ");

	for (l = 0; l < 3; l++)
	  fprintf (file, "%.12f ", Tess.VerCoo[ver][l]);
	fprintf (file, "&\n");
      }
    }

    fprintf (file, "\n");
  }

  fprintf (file, "ret\n");

  return;
}
