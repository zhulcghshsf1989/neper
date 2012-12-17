/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_init.h"

void
nem_init_remesh (char* mesh, char* tess, struct GEO* pGeo, struct NODES* pRNodes,
		  struct MESH* pRMesh0D, struct MESH* pRMesh1D,
		  struct MESH* pRMesh2D, struct MESH* pRMesh3D)
{
  FILE* file = NULL;

  ut_print_message (0, 2, "Loading mesh ...\n");

  file = ut_file_open (mesh, "r");
  neut_mesh_fscanf_msh (file, pRNodes, pRMesh0D, pRMesh1D,
                                       pRMesh2D, pRMesh3D);
  ut_file_close (file, mesh, "r");

  neut_mesh_init_eltelset (pRMesh2D, NULL);
  neut_mesh_init_nodeelts (pRMesh2D, (*pRNodes).NodeQty);
  neut_mesh_init_eltelset (pRMesh3D, NULL);
  neut_mesh_init_nodeelts (pRMesh3D, (*pRNodes).NodeQty);

  neut_nodes_init_bbox (pRNodes);

  if (tess == NULL)
  {
    ut_print_message (0, 2, "Reconstructing meshes and topology ...\n");
    nem_reconmesh ("3,2,1,0", pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, pGeo);
  }
  else
  {
    ut_print_message (0, 2, "Loading tessellation ...\n");
    file = ut_file_open (tess, "r");
    neut_geo_fscanf (file, pGeo);
    ut_file_close (file, tess, "r");
    ut_print_message (0, 2, "Testing tessellation ...\n");
    neut_geo_test (*pGeo, 0);
    
    ut_print_message (0, 2, "Reconstructing meshes ...\n");
    nem_reconmesh ("3,2,1,0", pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, NULL);
    
    nem_init_mesh_geo_updating (pGeo, *pRNodes, *pRMesh0D, *pRMesh1D, *pRMesh2D, *pRMesh3D);
  }

  return;
}

void
nem_init_scaling (char* elttype, struct GEO* pGeo, struct VOX* pVox, struct NODES* pRNodes,
    struct MESH RMesh0D, struct MESH RMesh1D, struct MESH RMesh2D,
    struct MESH RMesh3D, struct GEOPARA* pGeoPara)
{
  int i, polyqty;
  double* scale = NULL;
  double vol;
  char input;

  if ((*pGeo).PolyQty > 0)
  {
    input = 't'; // tess
    polyqty = (*pGeo).PolyQty;
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

  if ((*pGeoPara).cltype == 1 || (*pGeoPara).cltype == 3
   || ((*pGeoPara).dboundrcl > 0 && (*pGeoPara).dboundcl < 0))
  {
    if (input == 't')
      neut_geo_volume (*pGeo, &vol);
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
  if ((*pGeoPara).cltype == 1)
    rcl2cl ((*pGeoPara).rcl, vol, polyqty, elttype, &((*pGeoPara).cl));
  else if ((*pGeoPara).cltype == 2)
  {
    if (ut_num_equal ((*pGeoPara).cl3[0], (*pGeoPara).cl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).cl3[1], (*pGeoPara).cl3[2], 1e-12))
      (*pGeoPara).cl = (*pGeoPara).cl3[1];
    else
      (*pGeoPara).cl = pow ((*pGeoPara).cl3[0] * (*pGeoPara).cl3[1] *
			    (*pGeoPara).cl3[2], 0.333333333333333333333333333333333);

    for (i = 0; i < 3; i++)
      scale[i] = (*pGeoPara).cl / (*pGeoPara).cl3[i];

    if (input == 't')
      neut_geo_scale (pGeo, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);
  }
  else if ((*pGeoPara).cltype == 3)
  {
    if (ut_num_equal ((*pGeoPara).rcl3[0], (*pGeoPara).rcl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).rcl3[1], (*pGeoPara).rcl3[2], 1e-12))
      (*pGeoPara).rcl = (*pGeoPara).rcl3[1];
    else
      (*pGeoPara).rcl = pow ((*pGeoPara).rcl3[0] * (*pGeoPara).rcl3[1] *
			     (*pGeoPara).rcl3[2], 0.333333333333333333333333333333333);

    rcl2cl ((*pGeoPara).rcl, vol, polyqty, elttype, &((*pGeoPara).cl));

    for (i = 0; i < 3; i++)
      scale[i] = (*pGeoPara).rcl / (*pGeoPara).rcl3[i];

    if (input == 't')
      neut_geo_scale (pGeo, scale[0], scale[1], scale[2]);
    else if (input == 'v')
      neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);
    else
      neut_nodes_scale (pRNodes, scale[0], scale[1], scale[2]);

    if (input == 'm')
      nem_init_mesh_geo_updating
	(pGeo, *pRNodes, RMesh0D, RMesh1D, RMesh2D, RMesh3D);
  }
  
  /* Calculation of cl_skin */
  if ((*pGeoPara).dboundrcl > 0 && (*pGeoPara).dboundcl < 0)
    rcl2cl ((*pGeoPara).dboundrcl, vol, polyqty, elttype, &((*pGeoPara).dboundcl));

  if ((*pGeoPara).dbound != NULL && (*pGeoPara).dboundcl > 0 && (*pGeoPara).dboundcl < (*pGeoPara).cl)
  {
    ut_print_message (2, 0, "dboundcl = %f < cl = %f is not allowed.\n",
	              (*pGeoPara).dboundcl, (*pGeoPara).cl);
    ut_print_message (2, 0, "That means a refined mesh on the boundary; is that really what you want?  Not available.\n");
    abort ();
  }

  ut_free_1d (scale);
  
  return;
}

void
nem_post_scaling (struct GEOPARA GeoPara, struct GEO* pGeo,
                  struct VOX* pVox, struct NODES* pNodes)
{
  int i;

  if (GeoPara.cltype < 2)
    return;
  
  double* scale = ut_alloc_1d (3);

  if (GeoPara.cltype == 2)
    for (i = 0; i < 3; i++)
      scale[i] = GeoPara.cl3[i] / GeoPara.cl;
  else if (GeoPara.cltype == 3)
    for (i = 0; i < 3; i++)
      scale[i] = GeoPara.rcl3[i] / GeoPara.rcl;

  if ((*pGeo).PolyQty > 0)
    neut_geo_scale (pGeo, scale[0], scale[1], scale[2]);
    
  if ((*pVox).PolyQty > 0)
    neut_vox_scale (pVox, scale[0], scale[1], scale[2], NULL);

  neut_nodes_scale (pNodes, scale[0], scale[1], scale[2]);

  ut_free_1d (scale);

  return;
}
