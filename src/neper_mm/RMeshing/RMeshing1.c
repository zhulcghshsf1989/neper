/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "RMeshing.h"

void
RMeshing (struct NODES *pNodes, struct MESH *pMesh,
	  int ***pFoDNodes, struct IN In)
{
  int i;
  struct GEO Geo;
  struct VOX Vox;
  FILE *in = NULL;
  // char *expandnset = NULL;
  // char *expandfaset = NULL;
  int* msize = ut_alloc_1d_int (3);
  char* format = NULL;

  neut_geo_set_zero (&Geo);
  neut_vox_set_zero (&Vox);

  double** dsize = ut_alloc_2d (3, 2);
  
  // Input is a file
  if (In.ingeo != NULL)
  {
    // Checking format
    ut_file_format (In.ingeo, &format);
    if (format == NULL)
    {
      ut_print_message (2, 0, "Input file format unknown.\n");
      abort ();
    }

    // input if a tess file - reading tess
    if (! strcmp (format, "tess"))
    {
      ut_print_message (0, 2, "Loading tessellation ...\n");

      in = ut_file_open (In.ingeo, "r");
      neut_geo_fscanf (in, &Geo);
      ut_file_close (in, In.ingeo, "r");

      neut_geo_boundingbox (Geo, dsize);
    }

    // input if a vox file - reading vox
    else if (! strcmp (format, "vox"))
    {
      ut_print_message (0, 2, "Loading voxel data ...\n");

      in = ut_file_open (In.ingeo, "r");
      neut_vox_fscanf (in, &Vox);
      ut_file_close (in, In.ingeo, "r");

      neut_vox_boundingbox (Vox, dsize);
    }
    else
      abort ();
  }

  nemm_in_msize (In, dsize, msize);

  // neut_utils_nset_expand (In.nset, &expandnset);
  // neut_utils_nset_expand (In.faset, &expandfaset);

  /* Creating mapped mesh ------------------------------------------ */
  ut_print_message (0, 2, "Generating mesh ...\n");
  BuildMMesh3D (msize, In.morder, pNodes, pMesh, pFoDNodes);

  neut_nodes_scale (pNodes, dsize[0][1], dsize[1][1], dsize[2][1]);
  neut_mesh_scale  (pMesh, dsize[0][1], dsize[1][1], dsize[2][1]);

  /* Searching elsets ---------------------------------------------- */

  ut_print_message (0, 2, "Searching elsets ... ");
  int elsetqty, elset;
  if (! strcmp (format, "tess"))
  {
    RMeshingFGeo (Geo, pMesh);
    elsetqty = Geo.PolyQty;
  }
  else if (! strcmp (format, "vox"))
  {
    RMeshingFVox (Vox, In.cleaning, pMesh);
    elsetqty = Vox.PolyQty;
  }
  else
    abort ();

  (*pMesh).ElsetQty = elsetqty; 
  (*pMesh).Elsets = ut_alloc_2d_int ((*pMesh).ElsetQty + 1, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    elset = (*pMesh).EltElset[i];
    (*pMesh).Elsets[elset][0]++;
    (*pMesh).Elsets[elset]
      = ut_realloc_1d_int ((*pMesh).Elsets[elset], (*pMesh).Elsets[elset][0] + 1);
    (*pMesh).Elsets[elset][(*pMesh).Elsets[elset][0]] = i;
  }

  if ((Geo.DomType != NULL && strcmp (Geo.DomType, "cube") != 0)
      || Vox.PolyQty > 0)
  {
    ut_print_message (0, 2, "Removing 0-indexed elements ... \n");
    neut_mesh_rmelset (pMesh, *pNodes, 0);
    neut_nodes_rmorphans (pNodes, pMesh, pFoDNodes);
    ut_free_2d_int ((*pMesh).NodeElts, (*pNodes).NodeQty + 1);
    neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

    if (In.meshpoly != NULL && Vox.PolyQty > 0)
    {
      ut_print_message (0, 2, "Removing elsets other than `%s' ... \n", In.meshpoly);
      RMeshing_meshpoly (In.meshpoly, Vox, pMesh, pNodes, pFoDNodes);
    }

    if (In.singnodedup)
    {
      ut_print_message (0, 2, "Duplicating nodes with singularity ... ");
      RMeshing_singnodedup (pMesh, pNodes, pFoDNodes);
    }
  }

  // ut_free_1d_char (expandnset);
  // ut_free_1d_char (expandfaset);
  ut_free_1d_int (msize);
  ut_free_2d (dsize, 3);
  ut_free_1d_char (format);

  return;
}
