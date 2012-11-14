/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Miscellaneous.h"

void
DefGermSet (struct GERMSET *pGermSet)
{
  if (! strcmp ((*pGermSet).ttype, "standard"))
  {
    (*pGermSet).nSize = (*pGermSet).Size;
    (*pGermSet).nVol = 0;
    (*pGermSet).nN = 0;
    (*pGermSet).nGermsCoo = NULL;

    (*pGermSet).nGermRandId = NULL;
  }
  else if (! strcmp ((*pGermSet).ttype, "periodic"))
  {
    GermSetSize (pGermSet);
    GermSetGermsPer (pGermSet);
  }
  else if (! strcmp ((*pGermSet).ttype, "subdomain"))
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
CreateTess (struct GERMSET GermSet, double *Size, struct TESS *pTess)
{
  struct POLY Domain;
  struct POLY *Poly;

  CubeDomain (Size, &Domain);
  PolyComp (Domain, GermSet, &Poly, 0);
  Tessellation (GermSet, Poly, pTess);

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
