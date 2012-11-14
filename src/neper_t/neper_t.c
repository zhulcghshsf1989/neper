/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_t.h"

int
neper_t (int fargc, char **fargv, int argc, char **argv)
{
  /* *********************************************************
   * STRUCTURE AND VARIABLE DEFINITIONS  */
  /* See the structure files for details. */
  FILE *file = NULL;
  struct POLY Domain;
  struct GERMSET GermSet;
  struct POLY *Poly = NULL;
  struct TESS Tess;
  struct GEO Geo;
  struct IN In;
  int* perm = NULL;

  /* initialization */
  neut_geo_set_zero (&Geo);
  neut_tess_set_zero (&Tess);
  net_in_set_zero (&In);
  neut_germset_set_zero (&GermSet);
  neut_poly_set_zero (&Domain);

  ut_print_moduleheader ("-T", fargc, fargv, argc, argv);

  /* *********************************************************
   * PARAMETER, INPUT DATA AND VARIABLES INITIALIZATION  */
  /* InputData initializes all the parameters, input data and related
   * variables. */
  ut_print_message (0, 1, "Reading input data ...\n");
  InputData_t (&In, &GermSet, fargc, fargv, argc, argv);

  // Input = n,id for tessellation, or regular / tocta
  if (In.input == 0
  || (In.input == 2 && strcmp (GermSet.morpho, "tocta") == 0))
  {
    int i, iter;

    /* *********************************************************
     * DOMAIN COMPUTATION  */
    ut_print_message (0, 1, "Creating domain ...\n");
    DomainComp (In, GermSet, &Domain);

    /* *********************************************************
     * GERM DISTRIBUTION  */
    ut_print_message (0, 1, "Creating polyhedron centres ...\n");
    GermDistrib (In, Domain, &GermSet);

    char* message = ut_alloc_1d_char (1000);
    double rdistmax = -1;
    int status = 1;

    // Printing message
    if (In.centroid == 0)
      ut_print_message (0, 1, "Creating tessellation ... ");
    else
    {
      ut_print_message (0, 1,
	  "Creating centroidal tessellation ... (max = %d, conv = %.2f)\n",
	  In.centroiditermax, In.centroidconv);
      ut_print_message (0, 3, "iter  progress      conv\n");
      ut_print_message (0, 3, "");
    }

    for (iter = 1; (status == 1) &&
	           (iter <= ((In.centroid == 1) ? In.centroiditermax : 1)); iter++)
    {
      if (iter > 1)
      {
	for (i = 0; i < (int) strlen (message) + 4; i++)
	  printf ("\b");
      }

      if (In.centroid)
      {
	sprintf (message, "%4d      ", iter);
	printf ("%s", message);
      }

      // Calculation of all polyhedra, one by one
      PolyComp (Domain, GermSet, &Poly, 1);

      // Sticking polyhedra into a single tessellation
      if (iter > 1)
	neut_tess_free (&Tess);

      Tessellation (GermSet, Poly, &Tess);

      for (i = 1; i <= GermSet.N; i++)
	neut_poly_free (&(Poly[i]));
      free (Poly);

      if (In.centroid == 1)
	status = net_centroid (In, Tess, &GermSet, &rdistmax);
      else 
	status = 0;

      if (In.centroid)
      {
	printf ("   %7.2g", rdistmax);
	sprintf (message, "%s   %7.2g", message, rdistmax);
      }
    }
    
    printf ("\n");
    
    ut_free_1d_char (message);
  }

  // Importing tessellation
  else if (In.input == 1)
  {
    ut_print_message (0, 1, "Importing tessellation ...\n");
    file = ut_file_open (In.load, "r");
    neut_geo_fscanf_verbosity (file, &Geo, In.checktess);
    ut_file_close (file, In.load, "r");

    if (In.checktess)
      return 0;
  }
  
  if (Geo.PolyQty == 0)
  {
    neut_tess_geo (Tess, &Geo);
    neut_geo_init_domain_poly (&Geo, Domain, In.domain);
    // neut_tess_free (&Tess);
  }

  // from here, only geo.

  if (! ut_num_equal (In.F[0], 1, 1e-6)
   || ! ut_num_equal (In.F[1], 1, 1e-6)
   || ! ut_num_equal (In.F[2], 1, 1e-6))
  {
    ut_print_message (0, 1, "Deforming geometry ...\n");
    neut_geo_scale (&Geo, In.F[0], In.F[1], In.F[2]);
  }

  // neut_debug_geo (stdout, Geo);
  // abort ();
  
  if (In.sorttess_qty > 0)
  {
    ut_print_message (0, 1, "Sorting tessellation ...\n");
    net_geo_sort (&Geo, In.sorttess[0], In.sorttess[1], &perm);
  }

  /*
  ut_print_message (0, 1, "Testing tessellation ...\n");
    if (neut_geo_test (Geo) != 0)
      ut_error_reportbug ();
  */

/* *********************************************************
 * RESULTS WRITING  */
  ut_print_message (0, 1, "Writing results ...\n");
  Res_t (In, GermSet, Tess, Geo, perm);

  net_in_free (In);
  neut_germset_free (GermSet);
  neut_geo_free (&Geo);
  neut_tess_free (&Tess);
  neut_poly_free (&Domain);
  ut_free_1d_int (perm);

  return 0;
}
