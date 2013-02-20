/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess.h"

int
net_tess (int centroid, double centroidfact, int centroiditermax,
          double centroidconv, char* domain,
	  struct TESS PTess, int poly, struct GERMSET* pGermSet, int TessId,
	  struct TESS* pTess, int verbosity)
{
  struct POLY DomPoly;
  int i, iter;
  struct POLY* Poly = NULL;
  struct TESL Tesl;

  neut_poly_set_zero (&DomPoly);
  net_tess_poly (PTess, poly, &DomPoly);

  neut_tesl_set_zero (&Tesl);

  char* message = ut_alloc_1d_char (1000);
  double rdistmax = -1;
  int status = 1;

  for (iter = 1; (status == 1) &&
		 (iter <= ((centroid == 1) ? centroiditermax : 1)); iter++)
  {
    if (verbosity && iter > 1)
      for (i = 0; i < (int) strlen (message); i++)
	printf ("\b");

    // Calculation of all polyhedra, one by one ###
    net_polycomp (DomPoly, *pGermSet, &Poly, (verbosity && centroid
	  == 0) ? 1 : 0);

    // Sticking polyhedra into a single tessellation ###
    net_tesl (*pGermSet, Poly, &Tesl);

    // Free'ing Poly
    neut_poly_array_free (&Poly, (*pGermSet).N);

    // Moving GermSet to the centroids, if necessary
    if (centroid == 1)
      status = net_centroid (centroidfact, centroidconv, Tesl, pGermSet, &rdistmax);
    else
      status = 0;

    if (verbosity && centroid)
    {
      sprintf (message, "iter=%d, dist=%.3g", iter, rdistmax);
      printf ("%s", message);
      fflush (stdout);
    }
  }

  neut_tesl_tess (Tesl, pTess);
  (*pTess).Level  = PTess.Level + 1;
  (*pTess).TessId = TessId;
  neut_tesl_free (&Tesl);

  neut_tess_init_domain_poly (pTess, DomPoly, domain);

  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
  {
    ut_print_message (2, 2, "The tessellation is not valid.\n");
    ut_error_reportbug ();
  }
  
  ut_free_1d_char (message);
  neut_poly_free (&DomPoly);

  return 0;
}
