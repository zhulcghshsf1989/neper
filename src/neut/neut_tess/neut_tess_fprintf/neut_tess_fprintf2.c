/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_fprintf_lcl.h"

/* Tessellation exportation: head */
void
GEHead (struct TESS Tess, FILE * file)
{
  int i, qty;

  fprintf (file, "***tess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.0\n");
  fprintf (file, " **general\n");
  // fprintf (file, "   %d\n", Tess.Level);
  fprintf (file, "   %d %d %s %s\n", Tess.N, Tess.Id, Tess.Type, Tess.morpho);

  if (Tess.PolyId != NULL)
  {
    fprintf (file, "  *polyid\n");
    qty = 0;
    for (i = 1; i <= Tess.PolyQty; i++)
      ut_print_wnc_int_header (file, Tess.PolyId[i], &qty, 72, "  ");
    fprintf (file, "\n");
  }

  if (Tess.maxff > 0)
  {
    fprintf (file, "  *regularization\n");
    fprintf (file, "   %.12f %.12f", Tess.maxff, Tess.sel);
    if (Tess.dbound != NULL)
      fprintf (file, " %s %.12f\n", Tess.dbound, Tess.dboundsel);
    else
      fprintf (file, " nobound\n");
  }

  return;
}

/* Tessellation exportation: foot */
void
GEFoot (FILE * file)
{
  fprintf (file, "***end\n");

  return;
}

/* Tessellation exportation: vertex */
void
GEVer (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **vertex\n");
  fprintf (file, " %d\n", Tess.VerQty);

  for (i = 1; i <= Tess.VerQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Tess.VerDom[i], 2, "%d");

    fprintf (file, "    ");
    fprintf (file, " %d", Tess.VerEdgeQty[i]);
    for (j = 0; j < Tess.VerEdgeQty[i]; j++)
      fprintf (file, " %d", Tess.VerEdgeNb[i][j]);
    fprintf (file, "\n    ");
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Tess.VerCoo[i][j]);
    fprintf (file, "     %d\n", Tess.VerState[i]);
  }

  return;
}

/* Tessellation exportation: edge */
void
GEEdge (struct TESS Tess, FILE * file)
{
  int i;

  fprintf (file, " **edge\n");
  fprintf (file, " %d\n", Tess.EdgeQty);

  for (i = 1; i <= Tess.EdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Tess.EdgeDom[i], 2, "%d");

    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Tess.EdgeVerNb[i], 2, "%d");

    fprintf (file, "     %d ", Tess.EdgeFaceQty[i]);
    ut_array_1d_int_fprintf (file, Tess.EdgeFaceNb[i], Tess.EdgeFaceQty[i], "%d");
    fprintf (file, "     %d\n", Tess.EdgeState[i]);
  }

  return;
}

/* Tessellation exportation: face */
void
GEFace (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **face\n");
  fprintf (file, " %d\n", Tess.FaceQty);

  for (i = 1; i <= Tess.FaceQty; i++)
  {
    fprintf (file, " %3d ", i);
    ut_array_1d_int_fprintf (file, Tess.FaceDom[i], 2, "%d");

    fprintf (file, "    ");
    ut_array_1d_int_fprintf (file, Tess.FacePoly[i], 2, "%d");

    fprintf (file, "    ");
    ut_array_1d_fprintf (file, Tess.FaceEq[i], 4, "%.12f");

    fprintf (file, "    ");
    fprintf (file, " %d ", Tess.FaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Tess.FaceVerNb[i] + 1, Tess.FaceVerQty[i], "%d");

    fprintf (file, "      ");
    for (j = 1; j <= Tess.FaceVerQty[i]; j++)
      fprintf (file, " %d", Tess.FaceEdgeOri[i][j] * Tess.FaceEdgeNb[i][j]);
    fprintf (file, "\n");

    fprintf (file, "    ");
    fprintf (file, " %d", Tess.FaceState[i]);
    fprintf (file, " %d ", Tess.FacePt[i]);
    ut_array_1d_fprintf (file, Tess.FacePtCoo[i], 3, "%.12f");
    fprintf (file, "     %.12f", Tess.FaceFF[i]);
    fprintf (file, "\n");
  }

  return;
}

/* Tessellation exportation: poly */
void
GEPoly (struct TESS Tess, FILE * file)
{
  int i, j;

  fprintf (file, " **polyhedron\n");
  fprintf (file, " %d\n", Tess.PolyQty);

  for (i = 1; i <= Tess.PolyQty; i++)
  {
    /* polyhedron id */
    fprintf (file, " %3d", i);

    /* polyhedron center */
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f", Tess.CenterCoo[i][j]);
    fprintf (file, "\n");
    fprintf (file, "     %d %d", Tess.PolyTrue[i], Tess.PolyBody[i]);
    fprintf (file, "\n");

    /*
    // polyhedron vers
    fprintf (file, "     %d", Tess.PolyVerQty[i]);
    for (j = 1; j <= Tess.PolyVerQty[i]; j++)
      fprintf (file, " %d", Tess.PolyVerNb[i][j]);
    fprintf (file, "\n");

    // polyhedron edges
    fprintf (file, "     %d", Tess.PolyEdgeQty[i]);
    for (j = 1; j <= Tess.PolyEdgeQty[i]; j++)
      fprintf (file, " %d", Tess.PolyEdgeNb[i][j]);
    fprintf (file, "\n");
    */

    // polyhedron faces
    fprintf (file, "     %d", Tess.PolyFaceQty[i]);
    for (j = 1; j <= Tess.PolyFaceQty[i]; j++)
      fprintf (file, " %d", Tess.PolyFaceOri[i][j] * Tess.PolyFaceNb[i][j]);
    fprintf (file, "\n");
  }

  return;
}

/* Tessellation exportation: vertex */
void
GEDomain (struct TESS Tess, FILE * file)
{
  int i, j;

  if (Tess.Level == 0)
    return;

  fprintf (file, " **domain\n");
  fprintf (file, "  *general\n");
  fprintf (file, "   %s\n", Tess.DomType);

  fprintf (file, "  *vertex\n");
  fprintf (file, "   %d\n", Tess.DomVerQty);

  for (i = 1; i <= Tess.DomVerQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d\n", Tess.DomTessVerNb[i]);
    fprintf (file, "     ");
    fprintf (file, "%d ", Tess.DomVerEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomVerEdgeNb[i], Tess.DomVerEdgeQty[i], "%d");
  }

  fprintf (file, "  *edge\n");
  fprintf (file, "   %d\n", Tess.DomEdgeQty);

  for (i = 1; i <= Tess.DomEdgeQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d ", Tess.DomTessEdgeQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomTessEdgeNb[i] + 1, Tess.DomTessEdgeQty[i], "%d");
    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Tess.DomEdgeVerNb[i], 2, "%d");
    fprintf (file, "     ");
    ut_array_1d_int_fprintf (file, Tess.DomEdgeFaceNb[i], 2, "%d");
  }

  fprintf (file, "  *face\n");
  fprintf (file, "   %d\n", Tess.DomFaceQty);

  for (i = 1; i <= Tess.DomFaceQty; i++)
  {
    fprintf (file, " %3d ", i);
    fprintf (file, "%d ", Tess.DomTessFaceQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomTessFaceNb[i] + 1, Tess.DomTessFaceQty[i], "%d");

    fprintf (file, "     ");
    for (j = 1; j <= 3; j++)
      fprintf (file, "%.12f ", Tess.DomFaceEq[i][j]);
    fprintf (file, "%.12f ", Tess.DomFaceEq[i][0]);
    fprintf (file, "%s\n", Tess.DomFaceLabel[i]);

    fprintf (file, "   %3d ", Tess.DomFaceVerQty[i]);
    ut_array_1d_int_fprintf (file, Tess.DomFaceVerNb[i] + 1, Tess.DomFaceVerQty[i], "%d");
    fprintf (file, "       ");
    ut_array_1d_int_fprintf (file, Tess.DomFaceEdgeNb[i] + 1, Tess.DomFaceVerQty[i], "%d");
  }

  return;
}
