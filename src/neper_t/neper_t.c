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
    int message_length;
    double rdistmax = -1;
    int status = 1;

    for (iter = 1; (status == 1) &&
	           (iter <= ((In.centroid == 1) ? In.centroiditermax : 1)); iter++)
    {
      // Printing message
      if (In.centroid == 0)
	ut_print_message (0, 1, "Creating tessellation ... ");
      else
      {
	message_length = strlen (message);

	for (i = 0; i < message_length + 16; i++)
	  printf ("\b");

	sprintf (message, "Creating tessellation ... iter = %d(%d) - disp = %.2g(%.2g) - ", iter, In.centroiditermax, rdistmax, In.centroidconv);
	ut_print_message (0, 1, message);
	fflush (stdout);
      }

      /* *********************************************************
       * POLYEDRA DETERMINATION  */
      /* PolyComp decomposes the cube into the set of Voronoi polyhedra
       * and records them into Poly (which is allocated in the routine). */
      PolyComp (Domain, GermSet, &Poly, 1);

      /* *********************************************************
       * TESSELATION CONSTRUCTION  */
      /* Tessellation lies the polyhedra by suppressing vertices and faces
       * defined several times. That results in the Tess structure, which
       * contains all the information about the Voronoi tessellation. */
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
    }
    
    printf ("\n");
    
    ut_free_1d_char (message);
  }

  // Importing tessellation
  else if (In.input == 1)
  {
    ut_print_message (0, 1, "Importing tessellation ...\n");
    file = ut_file_open (In.load, "r");
    neut_geo_fscanf (file, &Geo);
    ut_file_close (file, In.load, "r");
    // printf ("Geo.FaceVerNb[1][1] = %d\n", Geo.FaceVerNb[1][1]);
    // abort ();
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
