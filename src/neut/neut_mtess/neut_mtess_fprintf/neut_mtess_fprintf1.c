/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fprintf_lcl.h"

void
neut_mtess_fprintf (FILE* file, struct MTESS MTess, struct TESS* Tess)
{
  int i;

  fprintf (file, "***mtess\n");
  fprintf (file, " **format\n");
  fprintf (file, "   2.0\n");
  fprintf (file, " **general\n");
  fprintf (file, "   %d\n", MTess.LevelQty);
  for (i = 1; i <= MTess.LevelQty; i++)
  {
    fprintf (file, "   %d ", MTess.LevelTessQty[i]);
    ut_array_1d_int_fprintf (file, MTess.LevelTess[i] + 1, MTess.LevelTessQty[i], "%d");
  }

  fprintf (file, "   %d\n", MTess.TessQty);
  for (i = 1; i <= MTess.TessQty; i++)
  {
    fprintf (file, "   ");
    ut_array_1d_int_fprintf (file, MTess.TessDom[i], 2, "%d");
  }

  fprintf (file, "***end\n");

  for (i = 0; i <= MTess.TessQty; i++)
    neut_tess_fprintf (file, Tess[i]);

  return;
}

