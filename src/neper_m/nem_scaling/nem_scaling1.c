/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_scaling.h"

void
nem_scaling_pre (char* elttype, struct TESS* pTess, struct VOX* pVox, struct NODES* pRNodes,
    struct MESH RMesh0D, struct MESH RMesh1D, struct MESH RMesh2D,
    struct MESH RMesh3D, struct MESHPARA* pMeshPara)
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
  else if (RMesh3D.ElsetQty + RMesh2D.ElsetQty + RMesh1D.ElsetQty + RMesh0D.ElsetQty > 0)
  {
    input = 'm'; // mesh
    polyqty = RMesh3D.ElsetQty;
  }
  else
    return;

  // If cltype is rcl or rcl3, data must be 3D
  if ((*pMeshPara).cltype == 1 || (*pMeshPara).cltype == 3)
    if (((input == 't' || input == 'm') && (*pTess).PolyQty == 0)
     || ((input == 'v') && (*pVox).PolyQty == 0))
     {
       ut_print_message (2, 0, "Option -rcl[3] cannot be used since input data is not 3D.\n");
       ut_print_message (2, 0, "Use -cl[3] instead.\n");
       abort ();
     }

  scale = ut_alloc_1d (3);

  if ((*pMeshPara).cltype == 1 || (*pMeshPara).cltype == 3
   || ((*pMeshPara).dboundrcl > 0 && (*pMeshPara).dboundcl < 0))
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
  if ((*pMeshPara).cltype == 1)
    rcl2cl ((*pMeshPara).rcl, vol, polyqty, elttype, &((*pMeshPara).cl));
  else if ((*pMeshPara).cltype == 2)
  {
    if (ut_num_equal ((*pMeshPara).cl3[0], (*pMeshPara).cl3[1], 1e-12)
     && ut_num_equal ((*pMeshPara).cl3[1], (*pMeshPara).cl3[2], 1e-12))
      (*pMeshPara).cl = (*pMeshPara).cl3[1];
    else
      (*pMeshPara).cl = pow ((*pMeshPara).cl3[0] * (*pMeshPara).cl3[1] *
			    (*pMeshPara).cl3[2], 0.333333333333333333333333333333333);

    for (i = 0; i < 3; i++)
      scale[i] = (*pMeshPara).cl / (*pMeshPara).cl3[i];

    if (input == 't')
      neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);
  }
  else if ((*pMeshPara).cltype == 3)
  {
    if (ut_num_equal ((*pMeshPara).rcl3[0], (*pMeshPara).rcl3[1], 1e-12)
     && ut_num_equal ((*pMeshPara).rcl3[1], (*pMeshPara).rcl3[2], 1e-12))
      (*pMeshPara).rcl = (*pMeshPara).rcl3[1];
    else
      (*pMeshPara).rcl = pow ((*pMeshPara).rcl3[0] * (*pMeshPara).rcl3[1] *
			     (*pMeshPara).rcl3[2], 0.333333333333333333333333333333333);

    rcl2cl ((*pMeshPara).rcl, vol, polyqty, elttype, &((*pMeshPara).cl));

    for (i = 0; i < 3; i++)
      scale[i] = (*pMeshPara).rcl / (*pMeshPara).rcl3[i];

    if (input == 't')
      neut_tess_scale (pTess, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);

    if (input == 'm')
      nem_tess_updatefrommesh_geom
	(pTess, *pRNodes, RMesh0D, RMesh1D, RMesh2D, RMesh3D);
  }

  /* Calculation of cl_skin */
  if ((*pMeshPara).dboundrcl > 0 && (*pMeshPara).dboundcl < 0)
    rcl2cl ((*pMeshPara).dboundrcl, vol, polyqty, elttype, &((*pMeshPara).dboundcl));

  if ((*pMeshPara).dbound != NULL && (*pMeshPara).dboundcl > 0 && (*pMeshPara).dboundcl < (*pMeshPara).cl)
  {
    ut_print_message (2, 0, "dboundcl = %f < cl = %f is not allowed.\n",
	              (*pMeshPara).dboundcl, (*pMeshPara).cl);
    ut_print_message (2, 0, "That means a refined mesh on the boundary; is that really what you want?  Not available.\n");
    abort ();
  }

  ut_free_1d (scale);

  return;
}

void
nem_scaling_post (struct MESHPARA MeshPara, struct TESS* pTess,
                  struct VOX* pVox, struct NODES* pNodes)
{
  int i;

  if (MeshPara.cltype < 2)
    return;

  double* scale = ut_alloc_1d (3);

  if (MeshPara.cltype == 2)
    for (i = 0; i < 3; i++)
      scale[i] = MeshPara.cl3[i] / MeshPara.cl;
  else if (MeshPara.cltype == 3)
    for (i = 0; i < 3; i++)
      scale[i] = MeshPara.rcl3[i] / MeshPara.rcl;

  if ((*pTess).PolyQty > 0)
    neut_tess_scale (pTess, scale[0], scale[1], scale[2]);

  if ((*pVox).PolyQty > 0)
    neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);

  neut_nodes_scale (pNodes, scale[0], scale[1], scale[2]);

  ut_free_1d (scale);

  return;
}
