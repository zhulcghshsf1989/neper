/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_germset.h"

void
neut_germset_set_zero (struct GERMSET* pGermSet)
{
  (*pGermSet).N = 0;
  (*pGermSet).N1d = 0;
  (*pGermSet).Id = 0;
  (*pGermSet).morpho = NULL;
  (*pGermSet).Random = 0;
  (*pGermSet).GermCoo = NULL;
  (*pGermSet).NDensity = 0;

  // Domain neighbourhood info
  (*pGermSet).Size = NULL;
  (*pGermSet).nN = 0;
  (*pGermSet).nSize = NULL;
  (*pGermSet).nVol = 0;
  (*pGermSet).nGermCoo = NULL;
  (*pGermSet).nGermRandId = NULL;
  (*pGermSet).nMaster = NULL;

  return;
}

void
neut_germset_free (struct GERMSET* pGermSet)
{
  ut_free_1d_char ((*pGermSet).morpho);
  ut_free_2d ((*pGermSet).GermCoo, (*pGermSet).N + 1);

  // Domain neighbourhood info
  ut_free_1d ((*pGermSet).Size);
  ut_free_1d ((*pGermSet).nSize);
  ut_free_2d ((*pGermSet).nGermCoo, (*pGermSet).nN);
  ut_free_1d_int ((*pGermSet).nGermRandId);
  ut_free_1d_int ((*pGermSet).nMaster);

  neut_germset_set_zero (pGermSet);

  return;
}
