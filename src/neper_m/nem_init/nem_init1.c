/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_init.h"

void
nem_init_remesh (struct TESS* pTess, struct NODES* pRNodes,
		 struct MESH* pRMesh0D, struct MESH* pRMesh1D,
		 struct MESH* pRMesh2D, struct MESH* pRMesh3D)
{
  neut_mesh_init_eltelset (pRMesh2D, NULL);
  neut_mesh_init_nodeelts (pRMesh2D, (*pRNodes).NodeQty);
  neut_mesh_init_eltelset (pRMesh3D, NULL);
  neut_mesh_init_nodeelts (pRMesh3D, (*pRNodes).NodeQty);

  if ((*pTess).PolyQty == 0)
  {
    ut_print_message (0, 2, "Reconstructing meshes and topology ...\n");
    nem_reconmesh ("3,2,1,0", pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, pTess);
  }
  else
  {
    ut_print_message (0, 2, "Reconstructing meshes ...\n");
    nem_reconmesh ("3,2,1,0", pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, NULL);
    
    nem_init_mesh_tess_updating (pTess, *pRNodes, *pRMesh0D, *pRMesh1D, *pRMesh2D, *pRMesh3D);
  }

  return;
}

void
nem_init_scaling (char* elttype, struct TESS* pTess, struct VOX* pVox, struct NODES* pRNodes,
    struct MESH RMesh0D, struct MESH RMesh1D, struct MESH RMesh2D,
    struct MESH RMesh3D, struct TESSPARA* pTessPara)
{
  int i, polyqty;
  double* scale = NULL;
  double vol;
  char input;

  if ((*pTess).PolyQty > 0)
  {
    input = 't'; // tess
    polyqty = (*pTess).PolyQty;
  }
  else if ((*pVox).PolyQty > 0)
  {
    input = 'v'; // vox
    polyqty = (*pVox).PolyQty;
  }
  else if (RMesh3D.EltQty > 0)
  {
    input = 'm'; // mesh
    polyqty = RMesh3D.ElsetQty;
  }
  else
    return;

  scale = ut_alloc_1d (3);

  if ((*pTessPara).cltype == 1 || (*pTessPara).cltype == 3
   || ((*pTessPara).dboundrcl > 0 && (*pTessPara).dboundcl < 0))
  {
    if (input == 't')
      neut_tess_volume (*pTess, &vol);
    else if (input == 'v')
      neut_vox_volume (*pVox, &vol);
    else
    {
      neut_mesh_volume (*pRNodes, RMesh3D, &vol);
      // Still necessary in Neper v2?
      // this is a poor workaround to avoid subsequent threshold problem
      // in the Meshing1D:  a segment of length 1 with cl 0.4999 would
      // be cut in 3 instead of 2... this cl problem can come from a
      // volume != 1.
      if (ut_num_equal (vol, 1, 1e-2))
	vol = 1;
    }
  }

  /* Calculation of cl */
  // cltype == 0: nothing to do
  if ((*pTessPara).cltype == 1)
    rcl2cl ((*pTessPara).rcl, vol, polyqty, elttype, &((*pTessPara).cl));
  else if ((*pTessPara).cltype == 2)
  {
    if (ut_num_equal ((*pTessPara).cl3[0], (*pTessPara).cl3[1], 1e-12)
     && ut_num_equal ((*pTessPara).cl3[1], (*pTessPara).cl3[2], 1e-12))
      (*pTessPara).cl = (*pTessPara).cl3[1];
    else
      (*pTessPara).cl = pow ((*pTessPara).cl3[0] * (*pTessPara).cl3[1] *
			    (*pTessPara).cl3[2], 0.333333333333333333333333333333333);

    for (i = 0; i < 3; i++)
      scale[i] = (*pTessPara).cl / (*pTessPara).cl3[i];

    if (input == 't')
      neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);
  }
  else if ((*pTessPara).cltype == 3)
  {
    if (ut_num_equal ((*pTessPara).rcl3[0], (*pTessPara).rcl3[1], 1e-12)
     && ut_num_equal ((*pTessPara).rcl3[1], (*pTessPara).rcl3[2], 1e-12))
      (*pTessPara).rcl = (*pTessPara).rcl3[1];
    else
      (*pTessPara).rcl = pow ((*pTessPara).rcl3[0] * (*pTessPara).rcl3[1] *
			     (*pTessPara).rcl3[2], 0.333333333333333333333333333333333);

    rcl2cl ((*pTessPara).rcl, vol, polyqty, elttype, &((*pTessPara).cl));

    for (i = 0; i < 3; i++)
      scale[i] = (*pTessPara).rcl / (*pTessPara).rcl3[i];

    if (input == 't')
      neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);

    if (input == 'm')
      nem_init_mesh_tess_updating
	(pTess, *pRNodes, RMesh0D, RMesh1D, RMesh2D, RMesh3D);
  }
  
  /* Calculation of cl_skin */
  if ((*pTessPara).dboundrcl > 0 && (*pTessPara).dboundcl < 0)
    rcl2cl ((*pTessPara).dboundrcl, vol, polyqty, elttype, &((*pTessPara).dboundcl));

  if ((*pTessPara).dbound != NULL && (*pTessPara).dboundcl > 0 && (*pTessPara).dboundcl < (*pTessPara).cl)
  {
    ut_print_message (2, 0, "dboundcl = %f < cl = %f is not allowed.\n",
	              (*pTessPara).dboundcl, (*pTessPara).cl);
    ut_print_message (2, 0, "That means a refined mesh on the boundary; is that really what you want?  Not available.\n");
    abort ();
  }

  ut_free_1d (scale);
  
  return;
}

void
nem_post_scaling (struct TESSPARA TessPara, struct TESS* pTess,
                  struct VOX* pVox, struct NODES* pNodes)
{
  int i;

  if (TessPara.cltype < 2)
    return;
  
  double* scale = ut_alloc_1d (3);

  if (TessPara.cltype == 2)
    for (i = 0; i < 3; i++)
      scale[i] = TessPara.cl3[i] / TessPara.cl;
  else if (TessPara.cltype == 3)
    for (i = 0; i < 3; i++)
      scale[i] = TessPara.rcl3[i] / TessPara.rcl;

  if ((*pTess).PolyQty > 0)
    neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    
  if ((*pVox).PolyQty > 0)
    neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);

  neut_nodes_scale (pNodes, scale[0], scale[1], scale[2]);

  ut_free_1d (scale);

  return;
}
