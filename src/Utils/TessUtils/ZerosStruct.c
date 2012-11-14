/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"ZerosStruct.h"

/* PolymodZeros: all the variables that are not used at the moment are
 * set to 0. */
void
PolymodZeros (int VerQty, int FaceQty, struct POLYMOD *pPolymod)
{
  twoDIntZeros ((*pPolymod).VerFace, (*pPolymod).VerQty + 1, VerQty, 0, 2);
  twoDDoubleZeros ((*pPolymod).VerCoo, (*pPolymod).VerQty + 1, VerQty, 0, 2);
  oneDIntZeros ((*pPolymod).VerUse, (*pPolymod).VerQty + 1, VerQty);
  twoDDoubleZeros ((*pPolymod).FaceEq, (*pPolymod).FaceQty + 1, FaceQty, 0, 3);
  oneDIntZeros ((*pPolymod).FacePoly, (*pPolymod).FaceQty + 1, FaceQty);
  oneDIntZeros ((*pPolymod).FaceVerQty, (*pPolymod).FaceQty + 1, FaceQty);
  twoDIntZeros ((*pPolymod).FaceVerNb, (*pPolymod).FaceQty + 1, FaceQty, 0,
		(*pPolymod).FaceQty + 1);

  return;
}
