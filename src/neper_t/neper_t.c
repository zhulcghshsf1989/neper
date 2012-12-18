/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_t.h"

int
neper_t (int fargc, char **fargv, int argc, char **argv)
{
  // Initializing variables --------------------------------------------
  
  struct IN In;
  struct TESSPARA TessPara;
  struct POLY Domain;
  struct GERMSET GermSet;
  struct TESS Tess;
  struct VOX Vox;
  FILE* file = NULL;
  char* action = NULL;

  neut_tess_set_zero     (&Tess);
  neut_vox_set_zero     (&Vox);
  net_in_set_zero       (&In);
  nem_tesspara_set_zero (&TessPara);
  neut_germset_set_zero (&GermSet);
  neut_poly_set_zero    (&Domain);

  // Printing header ---------------------------------------------------

  ut_print_moduleheader ("-T", fargc, fargv, argc, argv);

  // Reading input data ------------------------------------------------
 
  ut_print_message (0, 1, "Reading input data ...\n");
  InputData_t (&In, &TessPara, &action, fargc, fargv, argc, argv);

  if (ut_string_inlist (action, ',', "tessellate") == 1
   || ut_string_inlist (action, ',', "voxelize") == 1  )
  {
    ut_print_message (0, 1, "Creating domain ...\n");
    DomainComp (In, &Domain);

    ut_print_message (0, 1, "Creating polyhedron centres ...\n");
    GermDistrib (In, Domain, &GermSet);
    ut_print_message (0, 2, "%d centres created.\n", GermSet.N);
    if (GermSet.nN != 0)
      ut_print_message (0, 2, "%d centres created outside the domain.\n", GermSet.nN);
  /*
   * Init GermSet here 
  if (! strcmp ((*pIn).input, "n_reg"))
    net_n1d_morpho_n ((*pIn).N1d, parts[0], &((*pIn).n));
  */
  }

  // -------------------------------------------------------------------
  // Creating / loading + action on tessellation (depending on input) --
  // -------------------------------------------------------------------
 
  if (ut_string_inlist (action, ',', "tessellate") == 1
   || ! strcmp (In.input, "tess"))
  {
    if (ut_string_inlist (action, ',', "tessellate") == 1)
    {
      int i, iter;
      struct POLY* Poly = NULL;
      struct TESL Tesl;
      neut_tesl_set_zero (&Tesl);

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
	ut_print_message (0, 2, "iter      conv\n");
	ut_print_message (0, 2, "");
      }

      for (iter = 1; (status == 1) &&
		     (iter <= ((In.centroid == 1) ? In.centroiditermax : 1)); iter++)
      {
	if (iter > 1)
	{
	  for (i = 0; i < (int) strlen (message); i++)
	    printf ("\b");
	}

	if (In.centroid)
	{
	  sprintf (message, "%4d", iter);
	  printf ("%s", message);
	  fflush (stdout);
	}

	// Calculation of all polyhedra, one by one
	PolyComp (Domain, GermSet, &Poly, 0);

	// Sticking polyhedra into a single tessellation
	if (iter > 1)
	  neut_tesl_free (&Tesl);

	Tessellation (GermSet, Poly, &Tesl);

	for (i = 1; i <= GermSet.N; i++)
	  neut_poly_free (&(Poly[i]));
	free (Poly);
	Poly = NULL;

	if (In.centroid == 1)
	  status = net_centroid (In, Tesl, &GermSet, &rdistmax);
	else 
	  status = 0;

	if (In.centroid)
	{
	  printf ("  %8.3g", rdistmax);
	  fflush (stdout);
	  sprintf (message, "%s  %8.3g", message, rdistmax);
	}
      }
      
      printf ("\n");
      
      neut_tesl_tess (Tesl, &Tess);
      neut_tess_init_domain_poly (&Tess, Domain, In.domain);

      neut_tesl_free (&Tesl);
      
      ut_free_1d_char (message);
    }

    else if (! strcmp (In.input, "tesl"))
    {
      ut_print_message (0, 1, "Importing tessellation ...\n");
      file = ut_file_open (In.load, "r");
      neut_tess_fscanf_verbosity (file, &Tess, In.checktess);
      ut_file_close (file, In.load, "r");

      if (In.checktess)
	return 0;
    }

    if (In.sorttess_qty > 0)
    {
      ut_print_message (0, 1, "Sorting tessellation ...\n");
      net_tess_sort (&Tess, In.sorttess[0], In.sorttess[1]);
    }
  
    if (TessPara.maxff > 0)
    {
      net_init_reg (&Tess, &TessPara);
      RegularizeTess (&Tess, TessPara, In);
    }

    if (In.scale != NULL)
      neut_tess_scale (&Tess, In.scale[0], In.scale[1], In.scale[2]);
  
    // -------------------------------------------------------------------
    // Creating voxel data on tessellation (depending on input) ----------
    // -------------------------------------------------------------------

    if (ut_string_inlist (action, ',', "tess_voxelize") == 1)
    {
      ut_print_message (0, 1, "Voxelizing tessellation ... ");
      net_tess_vox (In, Tess, &Vox);
    }
  }

  // -------------------------------------------------------------------
  // Creating / loading + action on voxel data (depending on input) ----
  // -------------------------------------------------------------------
  
  else if (ut_string_inlist (action, ',', "voxelize") == 1
        || ! strcmp (In.input, "vox"))
  {
    if (ut_string_inlist (action, ',', "voxelize") == 1)
    {
      ut_print_message (0, 1, "Voxelizing domain ... \n");
      net_vox (In, Domain, GermSet, &Vox);
    }
    else if (! strcmp (In.input, "vox"))
    {
      ut_print_message (0, 1, "Importing voxel data ...\n");
      file = ut_file_open (In.load, "r");
      neut_vox_fscanf (file, &Vox);
      ut_file_close (file, In.load, "r");
      ut_print_message (0, 2, "Volume has %dx%dx%d (%g) %s and %d %s.\n",
			Vox.size[0], Vox.size[1], Vox.size[2],
			(double) (Vox.size[0] * Vox.size[1] * Vox.size[2]),
			(Vox.size[0] * Vox.size[1] * Vox.size[2] < 2) ? "voxel" : "voxels",
			Vox.PolyQty,
			(Vox.PolyQty < 2) ? "poly" : "polys");
    }
  }

  // -------------------------------------------------------------------
  // Action on voxel data (depending on input) -------------------------
  // -------------------------------------------------------------------
  
  // Scaling
  if ( Vox.PolyQty > 0 && In.scale != NULL)
      neut_vox_scale (&Vox, In.scale[0], In.scale[1], In.scale[2],
	  NULL);

  // Writing results ---------------------------------------------------
 
  ut_print_message (0, 1, "Writing results ...\n");
  Res_t (In, Tess, Vox);

  // Cleaning memory ---------------------------------------------------

  net_in_free (In);
  neut_germset_free (GermSet);
  neut_tess_free (&Tess);
  neut_vox_free (&Vox);
  neut_poly_free (&Domain);

  return 0;
}
