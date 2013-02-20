/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_op_lcl.h"

void
neut_mtess_set_zero (struct MTESS* pMTess)
{
  (*pMTess).LevelQty = 0;
  (*pMTess).LevelTessQty = NULL;
  (*pMTess).LevelTess = NULL;

  (*pMTess).TessQty = 0;
  (*pMTess).TessDom = NULL;

  return;
}

void
neut_mtess_free (struct MTESS MTess, struct TESS* Tess)
{
  int i;

  if (MTess.TessQty > 0)
    for (i = 0; i <= MTess.TessQty; i++)
      neut_tess_free (Tess + i);

  ut_free_1d_int (MTess.LevelTessQty);
  ut_free_2d_int (MTess.LevelTess, MTess.LevelQty + 1);
  ut_free_2d_int (MTess.TessDom, MTess.TessQty);

  return;
}

void
neut_mtess_set_tess (struct MTESS* pMTess, struct TESS** pTess)
{
  (*pMTess).LevelQty = 1;

  (*pMTess).LevelTessQty = ut_alloc_1d_int (2);
  ut_array_1d_int_set ((*pMTess).LevelTessQty, 2, 1);

  (*pMTess).LevelTess = ut_alloc_2d_int (2, 2);
  (*pMTess).LevelTess[1][1] = 1;

  (*pMTess).TessQty = 1;

  (*pMTess).TessDom = ut_alloc_2d_int (2, 2);
  ut_array_1d_int_set ((*pMTess).TessDom[1], 2, 1);

  if (pTess != NULL)
  {
    (*pTess) = malloc (2 * sizeof (TESS));
    neut_tess_set_zero (*pTess);
    neut_tess_set_zero (*pTess + 1);
  }

  return;
}
