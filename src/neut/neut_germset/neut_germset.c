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
  (*pGermSet).GermsCoo = NULL;
  (*pGermSet).ttype = NULL;
  (*pGermSet).NDensity = 0;
  (*pGermSet).randomize = 0;
  (*pGermSet).randomize2 = 0;
  (*pGermSet).randomizedir = NULL;

  // Domain neighbourhood info
  (*pGermSet).Size = NULL;
  (*pGermSet).nN = 0;
  (*pGermSet).nSize = NULL;
  (*pGermSet).nVol = 0;
  (*pGermSet).nGermsCoo = NULL;
  (*pGermSet).nGermRandId = NULL;
  (*pGermSet).GermToGerm = NULL;

  return;
}

void
neut_germset_free (struct GERMSET GermSet)
{
  ut_free_1d_char (GermSet.morpho);
  ut_free_2d (GermSet.GermsCoo, GermSet.N + 1);
  ut_free_1d_char (GermSet.randomizedir);
  ut_free_1d_char (GermSet.ttype);

  // Domain neighbourhood info
  ut_free_1d (GermSet.Size);
  ut_free_1d (GermSet.nSize);
  // ut_free_2d (GermSet.nGermsCoo);
  ut_free_1d_int (GermSet.nGermRandId);
  ut_free_1d_int (GermSet.GermToGerm);

  return;
}
