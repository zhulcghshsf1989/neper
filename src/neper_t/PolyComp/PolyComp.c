/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyComp.h"

/* PolyComp builds the set of Voronoi polyhedra from the set
 * of germ and the initial domain.
 */
void
PolyComp (struct INTEPARA *pIntePara, struct POLY Domain,
	  struct GERMSET GermSet,
	  struct POLY **pPoly, int verbosity)
{
  int center, m = 0;
  struct POLYMOD Polymod;

  /* Poly is the structure array containing the complete description
   * of all the polyhedra: Poly[i] contains the description of 
   * polyhedron whose center is germ i.
   */
  (*pPoly) = (POLY *) calloc (GermSet.N + 1, sizeof (POLY));

  /* The memory associated to the different 1D or 2D arrays of
   * the Polymod structure definition is allocated. This uses the
   * internal parameters and is independent of the morphology of
   * the initial polyhedron (cubic or not).
   */

  /* The polyhedra are calculated successively and independently,
   * from center 1 to center N.
   */
  for (center = 1; center <= GermSet.N; center++)
  {
    if (verbosity >= 1)
      PercentDisplay (center, GermSet.N, 10, &m);

    /* PolyhedronDetermination searches polyhedron n° center
     * and records it in Polymod.
     */
    PolymodAlloc (&Polymod);

    PolyhedronDetermination (pIntePara, center, Domain, GermSet, &Polymod);

    /* PolymodDescription(center,*pPolymod); */
    PolyhedronRecording (center, Polymod, *pPoly);
    /* PolyDescription(center,(*pPoly)[center]); */
    /* abort(); */
    PolymodFree (&Polymod);
  }

  return;
}
