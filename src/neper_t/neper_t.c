/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_t.h"

int
neper_t (int fargc, char **fargv, int argc, char **argv)
{
  // ###################################################################
  // ### INITIALIZING ##################################################

  // Variable declaration ###
  FILE* file = NULL;
  struct IN In;
  struct REG Reg;
  struct GERMSET GermSet;
  struct TESS* Tess = NULL;
  struct VOX Vox;

  net_in_set_zero       (&In);
  net_reg_set_zero      (&Reg);
  neut_germset_set_zero (&GermSet);
  neut_vox_set_zero     (&Vox);

  Tess = calloc (sizeof (struct TESS), 2);
  neut_tess_set_zero (&(Tess[0]));
  neut_tess_set_zero (&(Tess[1]));

  // Printing program header ###
  ut_print_moduleheader ("-T", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data ...\n");
  net_input (&In, &Reg, fargc, fargv, argc, argv);

  // ###################################################################
  // ### CREATING DOMAIN ###############################################

  // Creating domain ###
  if (strcmp (In.input, "tess") != 0 && strcmp (In.input, "vox") != 0)
  {
    ut_print_message (0, 1, "Creating domain ...\n");
    net_domain (In, Tess);
  }

  // ###################################################################
  // ### RUNNING MULTILEVEL TESSELLATION ###############################
  // ###################################################################

  if (! strcmp (In.mode, "tess"))
  {
    if (In.checktess == 1)
    {
      ut_print_message (0, 1, "Checking tessellation ...\n");
      file = ut_file_open (In.load, "r");
      neut_tess_fscanf_verbosity (file, &(Tess[1]), 1);
      ut_file_close (file, In.load, "r");

      return 0;
    }

    else if (In.nstring != NULL)
    {
      ut_print_message (0, 1, "Creating tessellation ... ");

      // ### CREATING GERMS ########################################

      net_germ (In, Tess[0], 1, &GermSet);

      // ### COMPUTING TESSELLATION ###############################

      if (strcmp (In.input, "tess") != 0)
	net_tess (In.centroid[1], In.centroidfact, In.centroiditermax,
		  In.centroidconv, In.domain,
		  Tess[0], 1, &GermSet, 1, &(Tess[1]),
		  (In.centroid[1] || GermSet.N >= 3000) ? 1 : 0);

      printf ("\n");

      // Sorting tessellation ###
      if (In.sorttess_qty > 0)
      {
	ut_print_message (0, 1, "[%5d] ", 1);
	printf ("Sorting polys with expression \"%s\" ...\n", In.sorttess[1]);
	if ((Tess[1]).PolyQty > 0)
	  neut_tess_sort (Tess + 1, In.sorttess[0], In.sorttess[1]);
      }

      if (In.scale != NULL)
      {
	ut_print_message (0, 1, "[%5d] ", 1);
	printf ("Scaling tessellation ...\n");
	neut_tess_scale (Tess + 1, In.scale[0], In.scale[1], In.scale[2]);
      }

    // #################################################################
      // ### REGULARIZING TESSELLATIONS ##############################

      if (Reg.reg[1] == 1)
      {
	ut_print_message (0, 1, "Regularizing tessellation ...\n");
	ut_print_message (0, 1, "  ");

	net_init_reg (&(Tess[1]), &Reg);
	net_regularization (Reg, &(Tess[1]));

	printf ("\n");
      }
    }

    else if (In.load != NULL)
    {
      ut_print_message (0, 1, "Importing tessellation ...\n");
      file = ut_file_open (In.load, "r");
      neut_tess_fscanf (file, &(Tess[1]));
      ut_file_close (file, In.load, "r");
    }
  }

  // ###################################################################
  // ### COMPUTING VOXEL DATA ##########################################

  else if (! strcmp (In.mode, "vox"))
  {
    if (strcmp (In.input, "vox") != 0)
    {
      ut_print_message (0, 1, "Voxelizing domain ... \n");
      net_germ (In, Tess[0], 1, &GermSet);
      net_vox (In, Tess[0], GermSet, &Vox);
    }

    else
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

      int i, j, k;
      int min_id = INT_MAX, max_id = INT_MIN;
      for (k = 1; k <= Vox.size[2]; k++)
	for (j = 1; j <= Vox.size[1]; j++)
	  for (i = 1; i <= Vox.size[0]; i++)
	  {
	    min_id = ut_num_min (min_id, Vox.VoxPoly[i][j][k]);
	    max_id = ut_num_max (max_id, Vox.VoxPoly[i][j][k]);
	  }

      if (min_id == 1 && max_id == Vox.PolyQty)
	ut_print_message (0, 3, "The polys are numbered contiguously from 1.\n");
      else
	ut_print_message (1, 3, "The poly ids range between %d and %d.\n",
	    min_id, max_id);
    }
  }

  // Scaling ###
  if (In.scale != NULL)
    if (Vox.PolyQty > 0)
      neut_vox_scale (&Vox, In.scale[0], In.scale[1], In.scale[2], NULL);

  // #################################################################
  // ### Voxelizing tessellation if necessary ########################

  if (Tess[1].PolyQty > 0
   && (ut_string_inlist (In.format, ',', "vox") == 1
    || ut_string_inlist (In.format, ',', "raw") == 1))
  {
    ut_print_message (0, 1, "Voxelizing tessellation ... ");
    net_tess_vox (In, Tess[1], &Vox);
  }

  // ###################################################################
  // ### WRITING RESULTS ###############################################

  ut_print_message (0, 1, "Writing results ...\n");
  net_res (In, Tess[1], Vox);

  // ###################################################################
  // ### CLOSING #######################################################

  net_in_free (In);
  net_reg_free (Reg);
  neut_germset_free (&GermSet);
  neut_vox_free     (&Vox);
  neut_tess_free (&(Tess[0]));
  neut_tess_free (&(Tess[1]));

  return EXIT_SUCCESS;
}
