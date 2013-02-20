/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_polycomp.h"

/* net_polycomp builds the set of Voronoi polyhedra from the set
 * of germ and the initial domain.
 */
void
net_polycomp (struct POLY Domain, struct GERMSET GermSet, struct POLY **pPoly, int verbosity)
{
  int i;
  char* message = ut_alloc_1d_char (10);

  // If *pPoly is not empty, free'ing
  neut_poly_array_free (pPoly, GermSet.N);

  // Alloc'ing Poly
  (*pPoly) = (POLY *) calloc (GermSet.N + 1, sizeof (POLY));

  // Calculating polys one by one
  for (i = 1; i <= GermSet.N; i++)
  {
    PolyhedronCalculation (Domain, GermSet, i, &((*pPoly)[i]));

    if (verbosity) 
      ut_print_progress_nonl (stdout, i, GermSet.N, "%3.0f%%", message);
  }

  ut_free_1d_char (message);

  return;
}
