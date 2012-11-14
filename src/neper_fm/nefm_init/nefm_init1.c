/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/* This file is ext of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nefm_init.h"

void
nefm_init_geo (char* filename, struct GEO* pGeo, struct GEOPARA* pGeoPara)
{
  double vol;
  FILE* file;

  ut_print_message (0, 2, "Loading tessellation ...\n");
  file = ut_file_open (filename, "r");
  neut_geo_fscanf (file, pGeo);
  ut_file_close (file, filename, "r");

  /* Calculation of cl */
  // cltype == 0: nothing to do
  if ((*pGeoPara).cltype == 1)
  {
    neut_geo_volume (*pGeo, &vol);
    rcl2cl ((*pGeoPara).rcl, vol, (*pGeo).PolyQty, &((*pGeoPara).cl));
  }
  else if ((*pGeoPara).cltype == 2)
  {
    if (ut_num_equal ((*pGeoPara).cl3[0], (*pGeoPara).cl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).cl3[1], (*pGeoPara).cl3[2], 1e-12))
      (*pGeoPara).cl = (*pGeoPara).cl3[1];
    else
      (*pGeoPara).cl = pow ((*pGeoPara).cl3[0] * (*pGeoPara).cl3[1] *
			    (*pGeoPara).cl3[2], 0.333333333333333333333333333333333);

    neut_geo_scale (pGeo, (*pGeoPara).cl / (*pGeoPara).cl3[0],
	(*pGeoPara).cl / (*pGeoPara).cl3[1],
	(*pGeoPara).cl / (*pGeoPara).cl3[2]);
  }
  else if ((*pGeoPara).cltype == 3)
  {
    if (ut_num_equal ((*pGeoPara).rcl3[0], (*pGeoPara).rcl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).rcl3[1], (*pGeoPara).rcl3[2], 1e-12))
      (*pGeoPara).rcl = (*pGeoPara).rcl3[1];
    else
      (*pGeoPara).rcl = pow ((*pGeoPara).rcl3[0] * (*pGeoPara).rcl3[1] *
			     (*pGeoPara).rcl3[2], 0.333333333333333333333333333333333);
    neut_geo_volume (*pGeo, &vol);
    rcl2cl ((*pGeoPara).rcl, vol, (*pGeo).PolyQty, &((*pGeoPara).cl));

    neut_geo_scale (pGeo, (*pGeoPara).rcl / (*pGeoPara).rcl3[0],
	(*pGeoPara).rcl / (*pGeoPara).rcl3[1],
	(*pGeoPara).rcl / (*pGeoPara).rcl3[2]);
  }
  
  /* Calculation of cl_skin */
  if ((*pGeoPara).dboundrcl > 0 &&
      (*pGeoPara).dboundcl < 0)
    rcl2cl ((*pGeoPara).dboundrcl, vol, (*pGeo).PolyQty, &((*pGeoPara).dboundcl));

  if ((*pGeoPara).dbound != NULL && (*pGeoPara).dboundcl > 0 && (*pGeoPara).dboundcl < (*pGeoPara).cl)
  {
    ut_print_message (2, 0, "dboundcl = %f < cl = %f is not allowed.\n",
	              (*pGeoPara).dboundcl, (*pGeoPara).cl);
    ut_print_message (2, 0, "That means a refined mesh on the boundary; is that really what you want?  Not available.\n");
    abort ();
  }
  
  cl2sel (pGeoPara);

  return;
}

void
nefm_init_remesh (struct IN In, struct GEO* pGeo, struct NODES* pRNodes,
    struct MESH* pRMesh0D, struct MESH* pRMesh1D, struct MESH* pRMesh2D,
    struct MESH* pRMesh3D, struct GEOPARA* pGeoPara)
{
  int i;
  double* def = ut_alloc_1d (3);
  double vol, volmin, volmax;
  FILE* file;

  ut_print_message (0, 2, "Loading mesh ...\n");

  file = ut_file_open (In.remesh2, "r");
  neut_mesh_fscanf_msh (file, pRNodes, pRMesh0D, pRMesh1D,
                                       pRMesh2D, pRMesh3D);
  ut_file_close (file, In.remesh2, "r");

  neut_mesh_init_eltelset (pRMesh2D, NULL);
  neut_mesh_init_nodeelts (pRMesh2D, (*pRNodes).NodeQty);
  neut_mesh_init_eltelset (pRMesh3D, NULL);
  neut_mesh_init_nodeelts (pRMesh3D, (*pRNodes).NodeQty);

  neut_nodes_init_boundingbox (pRNodes);

  if (In.remeshtess == NULL)
  {
    ReconMesh ("3,2,1,0", *pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, pGeo);
  }
  else
  {
    ut_print_message (0, 2, "Loading tessellation ...\n");
    file = ut_file_open (In.remeshtess, "r");
    neut_geo_fscanf (file, pGeo);
    ut_file_close (file, In.remeshtess, "r");
    ut_print_message (0, 2, "Testing tessellation ...\n");
    neut_geo_test (*pGeo);
    
    ReconMesh ("3,2,1,0", *pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, NULL);
    
    nefm_init_remeshingmapping_geo_updating (pGeo, *pRNodes, *pRMesh0D, *pRMesh1D, *pRMesh2D, *pRMesh3D);
  }

  /* Calculation of cl */
  // cltype == 0: nothing to do
  if ((*pGeoPara).cltype == 1)
  {
    neut_mesh_volume (*pRNodes, *pRMesh3D, &vol, &volmin, &volmax);
    rcl2cl ((*pGeoPara).rcl, vol, (*pGeo).PolyQty, &((*pGeoPara).cl));
  }
  else if ((*pGeoPara).cltype == 2)
  {
    if (ut_num_equal ((*pGeoPara).cl3[0], (*pGeoPara).cl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).cl3[1], (*pGeoPara).cl3[2], 1e-12))
      (*pGeoPara).cl = (*pGeoPara).cl3[1];
    else
      (*pGeoPara).cl = pow ((*pGeoPara).cl3[0] * (*pGeoPara).cl3[1] *
			    (*pGeoPara).cl3[2], 0.333333333333333333333333333333333);

    for (i = 0; i < 3; i++)
      def[i] = (*pGeoPara).cl / (*pGeoPara).cl3[i];
    neut_nodes_scale (pRNodes, def[0], def[1], def[2]);
  }
  else if ((*pGeoPara).cltype == 3)
  {
    if (ut_num_equal ((*pGeoPara).rcl3[0], (*pGeoPara).rcl3[1], 1e-12)
     && ut_num_equal ((*pGeoPara).rcl3[1], (*pGeoPara).rcl3[2], 1e-12))
      (*pGeoPara).rcl = (*pGeoPara).rcl3[1];
    else
      (*pGeoPara).rcl = pow ((*pGeoPara).rcl3[0] * (*pGeoPara).rcl3[1] *
			     (*pGeoPara).rcl3[2], 0.333333333333333333333333333333333);
    neut_mesh_volume (*pRNodes, *pRMesh3D, &vol, &volmin, &volmax);
    
    // this is a poor workaround to avoid subsequent threshold problem in the 
    // Meshing1D:  a segment of length 1 with cl 0.4999 would be cut in
    // 3 instead of 2... this cl problem can come from a volume != 1.
    if (ut_num_equal (vol, 1, 1e-2))
      vol = 1;

    rcl2cl ((*pGeoPara).rcl, vol, (*pGeo).PolyQty, &((*pGeoPara).cl));

    for (i = 0; i < 3; i++)
      def[i] =  (*pGeoPara).rcl / (*pGeoPara).rcl3[i];
    neut_nodes_scale (pRNodes, def[0], def[1], def[2]);

    // Recalculating geo (neut_geo_scale only is not enough at all).
    // neut_geo_free (pGeo);
    // ReconMesh ("3,2,1,0", *pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, pGeo);
  
    nefm_init_remeshingmapping_geo_updating (pGeo, *pRNodes, *pRMesh0D, *pRMesh1D, *pRMesh2D, *pRMesh3D);
  }

  /* Calculation of cl_skin */
  if ((*pGeoPara).dboundrcl > 0 &&
      (*pGeoPara).dboundcl < 0)
  {
    if (vol < 0)
      neut_mesh_volume (*pRNodes, *pRMesh3D, &vol, &volmin, &volmax);
    rcl2cl ((*pGeoPara).dboundrcl, vol, (*pGeo).PolyQty, &((*pGeoPara).dboundcl));
  }
  
  if ((*pGeoPara).dbound != NULL && (*pGeoPara).dboundcl > 0 && (*pGeoPara).dboundcl < (*pGeoPara).cl)
  {
    ut_print_message (2, 0, "dboundcl = %f < cl = %f is not allowed.\n",
	              (*pGeoPara).dboundcl, (*pGeoPara).cl);
    ut_print_message (2, 0, "That means a refined mesh on the boundary; is that really what you want?  Not available.\n");
    abort ();
  }
  
  cl2sel (pGeoPara);
  ut_free_1d (def);

  return;
}

void
nefm_init_remap (struct IN In, struct GEO* pGeo, struct NODES* pRNodes,
    struct MESH* pRMesh0D, struct MESH* pRMesh1D, struct MESH* pRMesh2D,
    struct MESH* pRMesh3D)
{
  ut_print_message (0, 2, "Loading mesh ...\n");
  FILE* file;

  file = ut_file_open (In.remap2, "r");
  neut_mesh_fscanf_msh (file, pRNodes, pRMesh0D, pRMesh1D,
				       pRMesh2D, pRMesh3D);
  ut_file_close (file, In.remap2, "r");

  neut_mesh_init_eltelset (pRMesh2D, NULL);
  neut_mesh_init_nodeelts (pRMesh2D, (*pRNodes).NodeQty);
  neut_mesh_init_eltelset (pRMesh3D, NULL);
  neut_mesh_init_nodeelts (pRMesh3D, (*pRNodes).NodeQty);

  neut_nodes_init_boundingbox (pRNodes);

  if (In.transporttess == NULL)
  {
    ReconMesh ("3,2,1,0", *pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, pGeo);
  }
  else
  {
    ut_print_message (0, 2, "Loading tessellation ...\n");
    file = ut_file_open (In.transporttess, "r");
    neut_geo_fscanf (file, pGeo);
    ut_file_close (file, In.transporttess, "r");
    ut_print_message (0, 2, "Testing tessellation ...\n");
    neut_geo_test (*pGeo);
    
    ReconMesh ("3,2,1,0", *pRNodes, pRMesh0D, pRMesh1D, pRMesh2D, pRMesh3D, NULL);
    
    nefm_init_remeshingmapping_geo_updating (pGeo, *pRNodes, *pRMesh0D, *pRMesh1D, *pRMesh2D, *pRMesh3D);
  }

  return;
}
