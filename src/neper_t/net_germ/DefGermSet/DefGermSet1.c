/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DefGermSet.h"

void
DefGermSet (struct IN In, struct GERMSET *pGermSet)
{
  if (! strcmp (In.ttype, "standard"))
  {
    (*pGermSet).nSize = ut_alloc_1d (7);
    ut_array_1d_memcpy ((*pGermSet).nSize + 1, 6, (*pGermSet).Size + 1);
    (*pGermSet).nVol = 0;
    (*pGermSet).nN = 0;
    (*pGermSet).nGermsCoo = NULL;

    (*pGermSet).nGermRandId = NULL;
  }
  else if (! strcmp (In.ttype, "periodic"))
  {
    GermSetSize (pGermSet);
    GermSetGermsPer (pGermSet);
  }
  else if (! strcmp (In.ttype, "subdomain"))
  {
    if ((*pGermSet).N != 0)
    {
      GermSetSize (pGermSet);
      GermSetGermsSD (pGermSet);
    }
    else
      GermSet4EmpD (pGermSet);
  }
  else
    abort ();

  return;
}

void
net_n1d_morpho_n (int n1d, char* morpho, int* pn)
{
  if (strcmp (morpho, "cube") == 0)
    (*pn) = n1d * n1d * n1d;
  else if (strcmp (morpho, "tocta") == 0)
    (*pn) =   (n1d - 1) * (n1d - 1) * (n1d - 1) + n1d * n1d * n1d;

  return;
}
