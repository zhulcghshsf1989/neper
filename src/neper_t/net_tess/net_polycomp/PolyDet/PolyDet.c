/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyDet.h"

/* PolyhedronDetermination searches polyhedron n° center
 * and records it in Polymod.
 */
void
PolyhedronDetermination (int Center,
			 struct POLY Domain, struct GERMSET GermSet,
			 struct POLYMOD *pPolymod)
{
  /* Initialization of the polyhedron, which is the whole cubic
   * domain to tesselate, at the moment.
   */

  neut_poly_polymod (Domain, pPolymod);
  /* PolyhedronCalculation searches out the polyhedron based on Center.
   */
  PolyhedronCalculation (Center, GermSet, pPolymod);

  return;
}
