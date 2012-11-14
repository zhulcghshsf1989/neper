/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Miscellaneous.h"

void
CreateTess (struct GERMSET GermSet, double *Size, struct TESS *pTess)
{
  struct POLY Domain;
  struct POLY* Poly = NULL;

  CubeDomain (Size, &Domain);
  PolyComp (Domain, GermSet, &Poly, 0);
  Tessellation (GermSet, Poly, pTess);

  return;
}
