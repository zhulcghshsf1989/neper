/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp.h"

/* PolyhedronCalculation searches out the polyhedron associated
* to the considered germ.
*/
void
PolyhedronCalculation (struct POLY Domain, struct GERMSET GermSet, int PolyId,
                       struct POLY* pPoly)
{
  int i;
  int* NeiId = ut_alloc_1d_int (GermSet.N + 1);
  struct POLYMOD Polymod;

  neut_polymod_set_zero (&Polymod);

  neut_poly_polymod (Domain, &Polymod);

  NeiSearching (GermSet, PolyId, NeiId);

  for (i = 2; i <= GermSet.N; i++)
  {
    // CurrentPolyTest checks if the germ is close enough to be able to
    // modify Polymod
    if (CurrentPolyTest (Polymod, GermSet.GermCoo[PolyId],
	  GermSet.GermCoo[NeiId[i]]) == 0)
      break;

    // NewPolyhedron modifies Polymod using the new germ
    NewPolyhedron (GermSet, PolyId, NeiId[i], &Polymod);
  }

  net_polymod_poly (Polymod, pPoly);

  neut_polymod_free (&Polymod);

  ut_free_1d_int (NeiId);

  return;
}
