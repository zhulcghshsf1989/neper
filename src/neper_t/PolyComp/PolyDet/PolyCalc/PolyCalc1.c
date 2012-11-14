/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"PolyCalc.h"

/* PolyhedronCalculation searches out the polyhedron associated
* to the considered germ.
*/
void
PolyhedronCalculation (int Center, struct GERMSET GermSet,
		       struct INTEPARA *pIntePara, struct POLYMOD *pPolymod)
{
  int i;			/* Mute variable */
  int qtytosort;

  /* Nei is the germ to take into account to build the new polyhedron. */
  int Nei;
  /* maxidist is the maximum distance between the polyhedron center (germ)
   * and its further vertex.
   * neidist is the distance between the polyhedron center (germ) and the
   * germ which is taken into account to modify the polyhedron
   */
  double maxidist, neidist;
  int *nbs = ut_alloc_1d_int (GermSet.N);
  double *dists = ut_alloc_1d (GermSet.N);

  qtytosort = ut_num_min ((*pIntePara).MaxToSort, GermSet.N);

  NeiSearching (GermSet, qtytosort, Center, nbs, dists);

  for (i = 1; i < qtytosort; i++)
  {
    maxidist = MaxiDist (Center, GermSet, *pPolymod);
    neidist = dists[i];

    /* If i takes the value qtytosort (which is expressed below), this */
    /* parameter is increased by a factor of 2                         */
    if (i == qtytosort - 1 && i != (GermSet.N - 1))
    {
      qtytosort = ut_num_min (qtytosort * 2, GermSet.N);
      NeiSearching (GermSet, qtytosort, Center, nbs, dists);
    }

    /* CurrentPolyTest checks if the polyhedron determination is ended or
     * not.
     */
    if (CurrentPolyTest (maxidist, neidist) == 0)
      break;

    /* At this point, the polyhedron may have to be modified.
     * Let's record the new neighbor to take into account           */
    Nei = nbs[i];

    /* NewPolyhedron modifies the current polyhedron to take into
     * account neighbour Nei.
     */
    NewPolyhedron (Center, Nei, GermSet, pPolymod);
  }

  ut_free_1d_int (nbs);
  ut_free_1d (dists);

  return;
}
