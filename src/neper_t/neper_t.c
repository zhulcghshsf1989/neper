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
  int i;
  FILE* file = NULL;
  struct IN In;
  struct REG Reg;
  struct GERMSET GermSet;
  struct TESS* Tess;
  struct VOX Vox;

  net_in_set_zero       (&In);
  net_reg_set_zero      (&Reg);
  neut_germset_set_zero (&GermSet);
  neut_vox_set_zero     (&Vox);

  Tess = calloc (sizeof (struct TESS), 2);
  for (i = 0; i < 2; i++)
    neut_tess_set_zero (&(Tess[i]));

  // Printing program header ###
  ut_print_moduleheader ("-T", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data ...\n");
  net_input (&In, &Reg, fargc, fargv, argc, argv);

  // ###################################################################
  // ### CREATING INPUTS: DOMAIN AND GERMS #############################

  // Creating domain ###
  if (strcmp (In.input, "tess") != 0 && strcmp (In.input, "vox") != 0)
  {
    ut_print_message (0, 1, "Creating domain ...\n");
    net_domain (In, &(Tess[0]));

    ut_print_message (0, 1, "Creating polyhedron centres ...\n");
    net_germ (In, Tess[0], &GermSet);
    ut_print_message (0, 2, "%d centres created.\n", GermSet.N);
    if (GermSet.nN != 0)
      ut_print_message (0, 2, "%d centres created outside the domain.\n", GermSet.nN);
  }

  // ###################################################################
  // ### COMPUTING TESSELLATION ########################################

  if (! strcmp (In.mode, "tess"))
  {
    if (strcmp (In.input, "tess") != 0)
    {
      ut_print_message (0, 1, "Creating tessellation ... ");
      net_tess (In.centroid, In.centroidfact, In.centroiditermax,
		In.centroidconv, In.domain, Tess[0], &GermSet, &(Tess[1]));
    }

    else
    {
      ut_print_message (0, 1, "Importing tessellation ...\n");
      file = ut_file_open (In.load, "r");
      neut_tess_fscanf_verbosity (file, &(Tess[1]), In.checktess);
      ut_file_close (file, In.load, "r");

      if (In.checktess)
	return 0;
    }
  }

  // ###################################################################
  // ### COMPUTING VOXEL DATA ##########################################
  
  else if (! strcmp (In.mode, "vox"))
  {
    if (strcmp (In.input, "vox") != 0)
    {
      ut_print_message (0, 1, "Voxelizing domain ... \n");
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
    }
  }

  // Sorting tessellation ###
  if (In.sorttess_qty > 0)
  {
    ut_print_message (0, 1, "Sorting polys with expression \"%s\" ...\n", 
		      In.sorttess[1]);
    if ((Tess[1]).PolyQty > 0)
      neut_tess_sort (&(Tess[1]), In.sorttess[0], In.sorttess[1]);
  }

  // Scaling ###
  if (In.scale != NULL)
  {
    if ((Tess[1]).PolyQty > 0)
      neut_tess_scale (&(Tess[1]), In.scale[0], In.scale[1], In.scale[2]);

    if (Vox.PolyQty > 0)
      neut_vox_scale (&Vox, In.scale[0], In.scale[1], In.scale[2], NULL);
  }
  
  // #################################################################
  // ### Regularizing tessellation ###################################

  if (Reg.maxff > 0)
  {
    if ((Tess[1]).PolyQty > 0)
    {
      net_init_reg (&(Tess[1]), &Reg);
      net_regularization (Reg, &(Tess[1]));
    }
  }

  // #################################################################
  // ### Voxelizing tessellation if necessary ########################

  if ((Tess[1]).PolyQty && ut_string_inlist (In.format, ',', "vox") == 1)
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
  neut_germset_free (GermSet);
  neut_vox_free     (&Vox);

  for (i = 0; i < 2; i++)
    neut_tess_free (&(Tess[i]));

  return EXIT_SUCCESS;
}
