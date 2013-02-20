/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ_rand.h"

void
net_germ_rand (struct IN In, struct POLY Dom, struct GERMSET *pGSet)
{
  if (! strcmp (In.ttype, "subdomain"))
  {
    (*pGSet).NDensity = (*pGSet).N;
    (*pGSet).N =
      neut_rand_poisson ((double) (*pGSet).NDensity, (int) (*pGSet).Random, 1);
  }

  (*pGSet).Size = ut_alloc_1d (7);
  neut_poly_bbox (Dom, (*pGSet).Size);

  (*pGSet).GermCoo = ut_alloc_2d ((*pGSet).N + 1, 3);

  IniGermCoo ((*pGSet).morpho, (*pGSet).N, (*pGSet).Random,
	       (*pGSet).Size, Dom, (*pGSet).GermCoo);

  return;
}
