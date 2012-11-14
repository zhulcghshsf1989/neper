/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Miscellaneous.h"

void
CalcNeighVol (struct GERMSET *pGermSet)
{
  (*pGermSet).nVol =
    ((*pGermSet).nSize[2] - (*pGermSet).nSize[1]) *
    ((*pGermSet).nSize[4] - (*pGermSet).nSize[3]) *
    ((*pGermSet).nSize[6] - (*pGermSet).nSize[5]) - 1;

  return;
}
