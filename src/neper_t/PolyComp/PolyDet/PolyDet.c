/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyDet.h"

/* PolyhedronDetermination searches polyhedron n° center
 * and records it in Polymod.
 */
void
PolyhedronDetermination (struct INTEPARA *pIntePara, int Center,
			 struct POLY Domain, struct GERMSET GermSet,
			 struct POLYMOD *pPolymod)
{
  /* Initialization of the polyhedron, which is the whole cubic
   * domain to tesselate, at the moment.
   */

  P2Pm (Domain, pPolymod);
  /* COMM 050412
   PolymodZeros ((*pIntePara).MaxPolyVerQty, (*pIntePara).MaxPolyFaceQty,
		pPolymod);
  */
  /* PolyhedronCalculation searches out the polyhedron based on Center.
   */
  PolyhedronCalculation (Center, GermSet, pIntePara, pPolymod);

  return;
}
