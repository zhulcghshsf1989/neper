/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fscanf_lcl.h"

void
neut_mtess_fscanf (FILE* file, struct MTESS* pMTess, struct TESS** pTess)
{
  int i, j, tess, status;

  // status == 0 if mtess and 1 if tess
  status = neut_mtess_fscanf_head (pMTess, file);

  if ((*pMTess).LevelQty > 1)
    ut_print_message (0, 3, "multilevel tessellation (%d levels).\n", (*pMTess).LevelQty);

  (*pTess) = calloc ((*pMTess).TessQty + 1, sizeof (struct TESS));

  for (i = status; i <= (*pMTess).TessQty; i++)
    neut_tess_fscanf (file, &((*pTess)[i]));

  // Setting TessId and Level in the Tessellations.  Should be read in
  // neut_tess_fscanf (i.e. written to the .tess) instead?
  for (i = status; i <= (*pMTess).TessQty; i++)
    (*pTess)[i].TessId = i;

  for (i = 1; i <= (*pMTess).LevelQty; i++)
    for (j = 1; j <= (*pMTess).LevelTessQty[i]; j++)
    {
      tess = (*pMTess).LevelTess[i][j];
      (*pTess)[tess].Level = i;
    }

  return;
}
