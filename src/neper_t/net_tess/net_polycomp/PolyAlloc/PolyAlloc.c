/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyAlloc.h"

/* PolymodAlloc: The memory associated to the several 1D or 2D arrays
 * that intervene in the Polymod structure definition is allocated.
 * This uses the internal parameters and is independent of the
 * morphology of the initial polyhedron (cubic or not).
 */
void
PolymodAlloc (struct POLYMOD *pPolymod)
{
  (*pPolymod).VerFace = NULL;
  (*pPolymod).VerCoo = NULL;
  (*pPolymod).VerUse = NULL;

  (*pPolymod).FacePoly = NULL;
  (*pPolymod).FaceEq = NULL;
  (*pPolymod).FaceVerQty = NULL;
  (*pPolymod).FaceVerNb = NULL;
  (*pPolymod).FaceUse = NULL;

  return;
}
