/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"P2Pm.h"

void
P2Pm (struct POLY Poly, struct POLYMOD *pPolymod)
{
  P2PmVerQty (Poly, pPolymod);
  P2PmFaceQty (Poly, pPolymod);
  P2PmVerProp (Poly, pPolymod);
  P2PmFaceProp (Poly, pPolymod);

  return;
}
