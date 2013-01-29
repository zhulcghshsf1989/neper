/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_tess.h"

int
net_tess (int centroid, double centroidfact, int centroiditermax,
          double centroidconv, char* domain,
	  struct TESS Domain, struct GERMSET* pGermSet, struct TESS* pTess)
{
  struct POLY DomainPoly;
  int i, iter;
  struct POLY* Poly = NULL;
  struct TESL Tesl;

  neut_poly_set_zero (&DomainPoly);
  net_tess_poly (Domain, 1, &DomainPoly);

  neut_tesl_set_zero (&Tesl);

  char* message = ut_alloc_1d_char (1000);
  double rdistmax = -1;
  int status = 1;

  // Printing message ###
  if (centroid == 1)
  {
    printf ("\n");
    ut_print_message (0, 1, "(centroidal, max = %d, conv = %.2f)\n", centroiditermax, centroidconv);
    ut_print_message (0, 2, "iter      conv\n");
    ut_print_message (0, 2, "");
  }

  for (iter = 1; (status == 1) &&
		 (iter <= ((centroid == 1) ? centroiditermax : 1)); iter++)
  {
    if (iter > 1)
    {
      for (i = 0; i < (int) strlen (message); i++)
	printf ("\b");
    }

    if (centroid)
    {
      sprintf (message, "%4d", iter);
      printf ("%s", message);
      fflush (stdout);
    }

    // Calculation of all polyhedra, one by one ###
    net_polycomp (DomainPoly, *pGermSet, &Poly, (centroid == 0) ? 1 : 0);

    // Sticking polyhedra into a single tessellation ###
    if (iter > 1)
      neut_tesl_free (&Tesl);

    net_tesl (*pGermSet, Poly, &Tesl);

    for (i = 1; i <= (*pGermSet).N; i++)
      neut_poly_free (&(Poly[i]));
    free (Poly);
    Poly = NULL;

    if (centroid == 1)
      status = net_centroid (centroidfact, centroidconv, Tesl, pGermSet, &rdistmax);
    else 
      status = 0;

    if (centroid)
    {
      printf ("  %8.3g", rdistmax);
      fflush (stdout);
      sprintf (message, "%s  %8.3g", message, rdistmax);
    }
  }

  printf ("\n");
  
  neut_tesl_tess (Tesl, pTess);
  neut_tesl_free (&Tesl);

  neut_tess_init_domain_poly (pTess, DomainPoly, domain);

  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
  {
    ut_print_message (2, 2, "The tessellation is not valid.\n");
    ut_error_reportbug ();
  }
  
  ut_free_1d_char (message);
  neut_poly_free (&DomainPoly);

  return 0;
}
