/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyFree.h"

/* PolymodFree: The memory associated to the several 1D or 2D arrays
 * that intervene in the Polymod structure definition is allocated.
 * This uses the internal parameters and is independent of the
 * morphology of the initial polyhedron (cubic or not).
 */
void
PolymodFree (struct POLYMOD *pPolymod)
{
  ut_free_2d_int ((*pPolymod).VerFace, (*pPolymod).VerQty + 1);
  ut_free_2d     ((*pPolymod).VerCoo , (*pPolymod).VerQty + 1);
  ut_free_1d_int ((*pPolymod).VerUse);

  ut_free_1d_int ((*pPolymod).FacePoly);
  ut_free_2d     ((*pPolymod).FaceEq  , (*pPolymod).FaceQty + 1);
  ut_free_1d_int ((*pPolymod).FaceVerQty);
  ut_free_2d_int ((*pPolymod).FaceVerNb , (*pPolymod).FaceQty + 1);
  ut_free_1d_int ((*pPolymod).FaceUse);

  return;
}
