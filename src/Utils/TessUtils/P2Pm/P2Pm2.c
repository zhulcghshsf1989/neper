/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"P2Pm.h"

void
P2PmVerQty (struct POLY Poly, struct POLYMOD *pPolymod)
{
  (*pPolymod).VerQty = Poly.VerQty;

  return;
}

void
P2PmFaceQty (struct POLY Poly, struct POLYMOD *pPolymod)
{
  (*pPolymod).FaceQty = Poly.FaceQty;

  return;
}

void
P2PmVerProp (struct POLY Poly, struct POLYMOD *pPolymod)
{
  P2PmVerFace (Poly, pPolymod);
  P2PmVerCoo (Poly, pPolymod);
  P2PmVerUse (pPolymod);

  return;
}

void
P2PmFaceProp (struct POLY Poly, struct POLYMOD *pPolymod)
{
  P2PmFacePoly (Poly, pPolymod);
  P2PmFaceEq (Poly, pPolymod);
  P2PmFaceVerQty (Poly, pPolymod);
  P2PmFaceVerNb (Poly, pPolymod);
  P2PmFaceUse (pPolymod);

  return;
}
