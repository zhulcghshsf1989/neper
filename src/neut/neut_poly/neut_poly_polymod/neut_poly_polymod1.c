/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_poly_polymod.h"

void
neut_poly_polymod (struct POLY Poly, struct POLYMOD *pPolymod)
{
  neut_poly_polymodVerQty (Poly, pPolymod);
  neut_poly_polymodFaceQty (Poly, pPolymod);
  neut_poly_polymodVerProp (Poly, pPolymod);
  neut_poly_polymodFaceProp (Poly, pPolymod);

  return;
}
