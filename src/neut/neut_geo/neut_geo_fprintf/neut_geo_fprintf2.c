/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_fprintf_lcl.h"

/* Geometry exportation: head */
void
GEHead (struct GEO Geo, FILE * file)
{
  int i, qty;

  fprintf (file, "***tess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.0\n");
  fprintf (file, " **general\n");
  fprintf (file, "   %d %d %s %s\n", Geo.N, Geo.Id, Geo.Type, Geo.morpho);

  if (Geo.PolyId != NULL)
  {
    fprintf (file, "  *polyid\n");
    qty = 0;
    for (i = 1; i <= Geo.PolyQty; i++)
      ut_print_wnc_int_header (file, Geo.PolyId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Geo.maxff > 0)
  {
    fprintf (file, "  *regularization\n");
    fprintf (file, "   %.12f %.12f", Geo.maxff, Geo.sel);
    if (Geo.dbound != NULL)
      fprintf (file, " %s %.12f\n", Geo.dbound, Geo.dboundsel);
    else
      fprintf (file, " nobound\n");
  }

  return;
}

/* Geometry exportation: foot */
void
GEFoot (FILE * file)
{
  fprintf (file, "***end\n");

  return;
}

/* Geometry exportation: vertex */
void
GEVer (struct GEO Geo, FILE * file)
{
  int i, j;

  fprintf (file, " **vertex\n");
  fprintf (file, " %d\n", Geo.VerQty);

  for (i = 1; i <= Geo.VerQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Geo.VerDom[i], 2, "%d");
    fprintf (file, "    ");
    fprintf (file, " %d", Geo.VerEdgeQty[i]);
    for (j = 0; j < Geo.VerEdgeQty[i]; j++)
      fprintf (file, " %d", Geo.VerEdgeNb[i][j]);
    fprintf (file, "\n    ");
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Geo.VerCoo[i][j]);
    fprintf (file, "     %d\n", Geo.VerState[i]);
  }

  return;
}

/* Geometry exportation: edge */
void
GEEdge (struct GEO Geo, FILE * file)
{
  int i;

  fprintf (file, " **edge\n");
  fprintf (file, " %d\n", Geo.EdgeQty);

  for (i = 1; i <= Geo.EdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Geo.EdgeDom[i], 2, "%d");
    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Geo.EdgeVerNb[i], 2, "%d");

    fprintf (file, "     %d ", Geo.EdgeFaceQty[i]);
    ut_array_1d_int_fprintf (file, Geo.EdgeFaceNb[i], Geo.EdgeFaceQty[i], "%d");
    fprintf (file, "     %d\n", Geo.EdgeState[i]);
  }

  return;
}

/* Geometry exportation: face */
void
GEFace (struct GEO Geo, FILE * file)
{
  int i, j;

  fprintf (file, " **face\n");
  fprintf (file, " %d\n", Geo.FaceQty);

  for (i = 1; i <= Geo.FaceQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Geo.FaceDom[i], 2, "%d");

    fprintf (file, "    ");
    ut_array_1d_int_fprintf (file, Geo.FacePoly[i], 2, "%d");

    fprintf (file, "    ");
    ut_array_1d_fprintf (file, Geo.FaceEq[i], 4, "%.12f");

    fprintf (file, "    ");
    fprintf (file, " %d ", Geo.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Geo.FaceVerNb[i] + 1, Geo.FaceVerQty[i], "%d");

    fprintf (file, "      ");
    for (j = 1; j <= Geo.FaceVerQty[i]; j++)
      fprintf (file, " %d", Geo.FaceEdgeOri[i][j] * Geo.FaceEdgeNb[i][j]);
    fprintf (file, "\n");

    fprintf (file, "    ");
    fprintf (file, " %d", Geo.FaceState[i]);
    fprintf (file, " %d ", Geo.FacePt[i]);
    ut_array_1d_fprintf (file, Geo.FacePtCoo[i], 3, "%.12f");
    fprintf (file, "     %.12f", Geo.FaceFF[i]);
    fprintf (file, "\n");
  }

  return;
}

/* Geometry exportation: poly */
void
GEPoly (struct GEO Geo, FILE * file)
{
  int i, j;

  fprintf (file, " **polyhedron\n");
  fprintf (file, " %d\n", Geo.PolyQty);

  for (i = 1; i <= Geo.PolyQty; i++)
  {
    /* polyhedron id */
    fprintf (file, " %3d", i);

    /* polyhedron center */
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Geo.CenterCoo[i][j]);
    fprintf (file, "\n");
    fprintf (file, "     %d %d", Geo.PolyTrue[i], Geo.PolyBody[i]);
    fprintf (file, "\n");

    /* polyhedron faces */
    fprintf (file, "     %d", Geo.PolyFaceQty[i]);
    for (j = 1; j <= Geo.PolyFaceQty[i]; j++)
      fprintf (file, " %d", Geo.PolyFaceOri[i][j] * Geo.PolyFaceNb[i][j]);
    fprintf (file, "\n");
  }

  return;
}

/* Geometry exportation: vertex */
void
GEDomain (struct GEO Geo, FILE * file)
{
  int i, j;

  fprintf (file, " **domain\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   %s\n", Geo.DomType);

  fprintf (file, "  *vertex\n");
  fprintf (file, "   %d\n", Geo.DomVerQty);

  for (i = 1; i <= Geo.DomVerQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d\n", Geo.DomTessVerNb[i]);
    fprintf (file, "     ");
    fprintf (file, "%d ", Geo.DomVerEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomVerEdgeNb[i], Geo.DomVerEdgeQty[i], "%d");
  }

  fprintf (file, "  *edge\n");
  fprintf (file, "   %d\n", Geo.DomEdgeQty);

  for (i = 1; i <= Geo.DomEdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d ", Geo.DomTessEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomTessEdgeNb[i] + 1, Geo.DomTessEdgeQty[i], "%d");
    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Geo.DomEdgeVerNb[i], 2, "%d");
    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Geo.DomEdgeFaceNb[i], 2, "%d");
  }

  fprintf (file, "  *face\n");
  fprintf (file, "   %d\n", Geo.DomFaceQty);

  for (i = 1; i <= Geo.DomFaceQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d ", Geo.DomTessFaceQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomTessFaceNb[i] + 1, Geo.DomTessFaceQty[i], "%d");

    fprintf (file, "     ");
    for (j = 1; j <= 3; j++)
      fprintf (file, "%.12f ", Geo.DomFaceEq[i][j]);
    fprintf (file, "%.12f ", Geo.DomFaceEq[i][0]);
    fprintf (file, "%s\n", Geo.DomFaceLabel[i]);

    fprintf (file, "   %3d ", Geo.DomFaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Geo.DomFaceVerNb[i] + 1, Geo.DomFaceVerQty[i], "%d");
    fprintf (file, "       ");
    ut_array_1d_int_fprintf (file, Geo.DomFaceEdgeNb[i] + 1, Geo.DomFaceVerQty[i], "%d");
  }

  return;
}
