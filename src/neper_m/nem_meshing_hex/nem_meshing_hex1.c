/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_meshing_hex.h"

void
nem_tess_mesh_hex (struct IN In, struct TESSPARA TessPara, struct TESS Tess, 
		  struct NODES *pNodes, struct MESH* pMesh0D,
		  struct MESH* pMesh1D, struct MESH* pMesh2D, 
		  struct MESH *pMesh3D, struct NSET* pNSet2D)
{
  int i;
  int* msize = ut_alloc_1d_int (3);
  double** dsize = ut_alloc_2d (3, 2);

  neut_tess_bbox (Tess, dsize);

  double cl;
  nem_meshing_3D_poly_cl (TessPara, Tess, 1, &cl);

  for (i = 0; i < 3; i++)
    msize[i] = (dsize[i][1] - dsize[i][0]) / cl;

  // nemm_in_msize (In, dsize, msize);

  // neut_utils_nset_expand (In.nset, &expandnset);
  // neut_utils_nset_expand (In.faset, &expandfaset);

  /* Creating mapped mesh ------------------------------------------ */
  printf ("\n");
  ut_print_message (0, 2, "3D meshing ... ");

  neut_mesh_quad (msize, In.morder, pNodes, pMesh3D, pNSet2D);
  neut_nodes_scale (pNodes, dsize[0][1], dsize[1][1], dsize[2][1]);
  neut_mesh_scale  (pMesh3D, dsize[0][1], dsize[1][1], dsize[2][1]);

  /* Searching elsets ---------------------------------------------- */

  // ut_print_message (0, 2, "Searching elsets ... ");
  nem_meshing_hexFTess (Tess, pMesh3D);

  neut_mesh_init_elsets (pMesh3D);

  neut_mesh_rmelset (pMesh3D, *pNodes, 0);
  neut_nodes_rmorphans (pNodes, pMesh3D, pNSet2D);

  nem_reconmesh (In.outdim, pNodes, pMesh0D, pMesh1D, pMesh2D,
		        pMesh3D, NULL);

  int* meshpoly = ut_alloc_1d_int (Tess.PolyQty + 1);

  ut_free_2d_int ((*pMesh3D).NodeElts, (*pNodes).NodeQty + 1);
  neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);

  if (In.meshpoly != NULL)
  {
    ut_print_message (0, 2, "Removing elsets other than `%s' ... \n", In.meshpoly);
    neut_tess_expr_polytab (Tess, In.meshpoly, meshpoly);
    for (i = 1; i <= Tess.PolyQty; i++)
      if (meshpoly[i] == 0)
	neut_mesh_rmelset (pMesh3D, *pNodes, i);
  }

    /*
    if (In.singnodedup)
    {
      ut_print_message (0, 2, "Duplicating nodes with singularity ... ");
      nem_meshing_hex_singnodedup (pMesh3D, pNodes, pFoDNodes);
    }
    */

  // ut_free_1d_char (expandnset);
  // ut_free_1d_char (expandfaset);
  ut_free_1d_int (msize);
  ut_free_2d (dsize, 3);

  return;
}

void
nem_vox_mesh_hex (struct IN In, struct TESSPARA TessPara, struct VOX Vox,
                  struct NODES* pNodes, struct MESH* pMesh0D, struct
		  MESH* pMesh1D, struct MESH* pMesh2D, struct MESH* pMesh3D,
		  struct NSET* pNSet2D)

{
  int i, j, k, elt;
  struct VOX Vox2;
  int* msize = ut_alloc_1d_int (3);
  double* scale = ut_alloc_1d (3);
  double** dsize = ut_alloc_2d (3, 2);
  struct TESS Tess;

  neut_tess_set_zero (&Tess);

  neut_vox_bbox (Vox, dsize);

  double cl;
  nem_meshing_3D_poly_cl (TessPara, Tess, 1, &cl);

  for (i = 0; i < 3; i++)
    msize[i] = ut_num_max_int ((dsize[i][1] - dsize[i][0]) / cl, 1);

  // neut_utils_nset_expand (In.nset, &expandnset);
  // neut_utils_nset_expand (In.faset, &expandfaset);

  /* Creating mapped mesh ------------------------------------------ */
  printf ("\n");
  ut_print_message (0, 2, "3D meshing ... ");

  neut_mesh_quad (msize, In.morder, pNodes, pMesh3D, pNSet2D);
  neut_nodes_scale (pNodes, dsize[0][1], dsize[1][1], dsize[2][1]);
  neut_mesh_scale  (pMesh3D, dsize[0][1], dsize[1][1], dsize[2][1]);

  /*
    // input if a vox file - reading vox
    else if (! strcmp (format, "vox"))
    {
      ut_print_message (0, 2, "Loading voxel data ...\n");

      in = ut_file_open (In.tess, "r");
      neut_vox_fscanf (in, &Vox);
      ut_file_close (in, In.tess, "r");

      neut_vox_bbox (Vox, dsize);
    }
    else
      abort ();
  */

  printf ("\n");
  (*pMesh3D).msize = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pMesh3D).msize, 3, msize);

  for (i = 0; i < 3; i++)
    scale[i] = (double) (*pMesh3D).msize[i] / Vox.size[i];

  neut_vox_set_zero (&Vox2);
  neut_vox_memcpy (Vox, &Vox2);
  neut_vox_gridscale (&Vox2, scale[0], scale[1], scale[2]);

  elt = 0;
  for (k = 1; k <= (*pMesh3D).msize[2]; k++)
    for (j = 1; j <= (*pMesh3D).msize[1]; j++)
      for (i = 1; i <= (*pMesh3D).msize[0]; i++)
	(*pMesh3D).EltElset[++elt] = Vox2.VoxPoly[i - 1][j - 1][k - 1];

  neut_mesh_init_elsets (pMesh3D);

  neut_mesh_rmelset (pMesh3D, *pNodes, 0);
  neut_nodes_rmorphans (pNodes, pMesh3D, pNSet2D);

  if (In.meshpoly != NULL)
    nem_meshing_hex_meshpoly (In.meshpoly, Vox, pMesh3D, pNodes, pNSet2D);

  nem_reconmesh (In.outdim, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D,
      NULL);

  ut_free_1d_int (msize);
  ut_free_1d (scale);
  neut_tess_free (&Tess);
  neut_vox_free (&Vox2);

  return;
}
