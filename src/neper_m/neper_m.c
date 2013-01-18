/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neper_m.h"

int
neper_m (int fargc, char **fargv, int argc, char **argv)
{
  // ###################################################################
  // ### INITIALIZING ##################################################

  // Variable declaration ###
  struct IN In;
  struct TESS Tess;
  struct VOX Vox;
  struct MESHPARA MeshPara;
  struct NODES Nodes, RNodes;
  struct MESH Mesh0D, Mesh1D, Mesh2D, Mesh3D;
  struct MESH RMesh0D, RMesh1D, RMesh2D, RMesh3D;
  struct PART Part;
  struct NSET NSet0D, NSet1D, NSet2D;

  nem_in_set_zero     (&In);
  nem_meshpara_set_zero (&MeshPara);
  neut_tess_set_zero  (&Tess);
  neut_vox_set_zero   (&Vox);
  neut_nodes_set_zero (&RNodes);
  neut_mesh_set_zero  (&RMesh0D);
  neut_mesh_set_zero  (&RMesh1D);
  neut_mesh_set_zero  (&RMesh2D);
  neut_mesh_set_zero  (&RMesh3D);
  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero  (&Mesh0D);
  neut_mesh_set_zero  (&Mesh1D);
  neut_mesh_set_zero  (&Mesh2D);
  neut_mesh_set_zero  (&Mesh3D);
  neut_part_set_zero  (&Part);
  neut_nset_set_zero  (&NSet0D);
  neut_nset_set_zero  (&NSet1D);
  neut_nset_set_zero  (&NSet2D);
  
  // Printing program header ###
  ut_print_moduleheader ("-M", fargc, fargv, argc, argv);

  // Reading input data ###
  ut_print_message (0, 1, "Reading input data ...\n");
  nem_input (&In, &MeshPara, fargc, fargv, argc, argv);

  // ###################################################################
  // ### LOADING INPUT DATA ############################################

  // Loading tessellation ###
  if (In.tess != NULL)
  {
    ut_print_message (0, 1, "Loading tessellation ...\n");
    neut_tess_name_fscanf (In.tess, &Tess);
  }
  
  // Loading voxel data ###
  else if (In.vox != NULL)
  {
    ut_print_message (0, 1, "Loading voxel data ...\n");
    neut_vox_name_fscanf (In.vox, &Vox);

    if (! strcmp (In.elttype, "tri"))
    {
      ut_print_message (0, 1, "Reconstructing topology ...\n");
      nem_reconstruct_vox (Vox, &Tess, &RNodes, &RMesh1D, &RMesh2D, &NSet2D);
    }
  }

  // Loading input mesh (remeshing and / or transport) ###
  if (In.mesh != NULL)
  {
    ut_print_message (0, 1, "Loading mesh ...\n");
    neut_mesh_name_fscanf_msh (In.mesh, &RNodes, &RMesh0D, &RMesh1D, &RMesh2D, &RMesh3D);

    if (! strcmp (In.elttype, "tri"))
    {
      ut_print_message (0, 1, "Reconstructing topology ...\n");
      nem_reconstruct_mesh ("0,1,2,3", &RNodes, &RMesh0D, &RMesh1D, &RMesh2D, &RMesh3D, &Tess);
    }
  }

  // Pre-scaling of input if necessary (use of cl3/rcl3) ###
  nem_scaling_pre (In.elttype, &Tess, &Vox, &RNodes, RMesh0D, RMesh1D, RMesh2D, RMesh3D, &MeshPara);

  // ###################################################################
  // ### COMPUTING OUTPUT MESH #########################################

  if (Tess.PolyQty + Tess.FaceQty + Tess.EdgeQty + Tess.VerQty > 0 || Vox.PolyQty > 0)
  {
    ut_print_message (0, 1, "Meshing ...");

    if (! strcmp (In.elttype, "tri"))
      nem_meshing (In, MeshPara, Tess, RNodes, RMesh1D, RMesh2D, &Nodes,
		   &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D);
    
    else if (! strcmp (In.elttype, "quad"))
    {
      if (In.tess != NULL)
	nem_meshing_tess_quad (In, MeshPara, Tess, &Nodes, &Mesh0D, &Mesh1D,
	    &Mesh2D, &Mesh3D, &NSet2D);
      else if (In.vox != NULL)
	nem_meshing_vox_quad (In, MeshPara, Vox, &Nodes, &Mesh0D, &Mesh1D,
	    &Mesh2D, &Mesh3D, &NSet2D);
    }
  }

  // Post-scaling of input / mesh if necessary (use of cl3/rcl3) ###
  nem_scaling_post (MeshPara, &Tess, &Vox, &Nodes);

  // Loading mesh (mutually exclusive with the 2 previous ones) ###
  if (In.loadmesh != NULL)
  {
    ut_print_message (0, 1, "Loading mesh ...\n");
    nem_readmesh (In.loadmesh, In.nodecoo, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D);
  }

  if (In.singnodedup)
  {
    ut_print_message (0, 1, "Duplicating singular nodes ... ");
    nem_cleaning_singnodedup (&Mesh3D, &Nodes, NULL);
  }

  if (In.dupnodemerge > 0)
  {
    ut_print_message (0, 1, "Merging duplicated nodes ... ");
    nem_cleaning_dupnodemerge (&Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D,
	In.dupnodemerge);
  }

  // Reconstructing mesh if necessary (2D, 1D, 0D from 3D) ###
  // Should not be necessary for cases other that -loadmesh.
  if (In.loadmesh != NULL
   && ((ut_string_inlist (In.outdim, ',', "2") && Mesh2D.EltQty == 0)
   ||  (ut_string_inlist (In.outdim, ',', "1") && Mesh1D.EltQty == 0)
   ||  (ut_string_inlist (In.outdim, ',', "0") && Mesh0D.EltQty == 0)
   ||  (Tess.PolyQty == 0 && strlen (In.nset) > 0)))
  {
    ut_print_message (0, 1, "Reconstructing mesh ...\n");
    nem_reconstruct_mesh (In.outdim, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D,
			  &Mesh3D, &Tess);
  }

  // managing mesh order ###
  if (Mesh3D.EltQty > 0 && Mesh3D.EltOrder == 1 && In.morder == 2)
  {
    ut_print_message (0, 2, "Switching mesh to order 2 ...\n");
    neut_mesh_order1to2 (&Nodes, &Mesh1D, &Mesh2D, &Mesh3D);
  }

  // ###################################################################
  // ### POST-MESHING OPERATIONS #######################################

#ifdef HAVE_LIBSCOTCH
  // Partitioning mesh ###
  if (In.partmode != -1)
  {
    ut_print_message (0, 1, "Partitioning mesh ...\n");
    nem_partition (In, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, &Part);
  }
#endif

  // Searching node sets ###
  if (strlen (In.nset) > 0 && strcmp (In.nset, "none") != 0 && Mesh3D.EltQty > 0)
  {
    ut_print_message (0, 2, "Searching nsets ...\n");
    if (Mesh2D.ElsetQty > 0)
      nem_nsets (In, Tess, Mesh0D, Mesh1D, Mesh2D, &NSet0D, &NSet1D, &NSet2D);
    else
      ut_print_message (1, 3, "2D mesh is void.  Skipping ...\n");
  }

  // ###################################################################
  // WRITING MESH ######################################################

  if (Mesh0D.EltQty + Mesh1D.EltQty + Mesh2D.EltQty + Mesh3D.EltQty > 0)
  {
    ut_print_message (0, 1, "Writing mesh results ...\n");
    nem_writemesh (In, Tess, Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, NSet0D, NSet1D, NSet2D, Part);
  }

  // ###################################################################
  // ### DATA TRANSPORT (GOES WITH REMESHING) ##########################

  if (In.transportqty > 0)
  {
    ut_print_message (0, 1, "Transporting data ...\n");
    nem_transport (In, Tess, RNodes, RMesh2D, RMesh3D, &Nodes, &Mesh2D, &Mesh3D);
  }

  // ###################################################################
  // ### MESH STATISTICS ###############################################

  if (In.stn != NULL || In.ste != NULL || In.stelset != NULL)
  {
    ut_print_message (0, 1, "Writing mesh statistics ...\n");
    if (Tess.PolyTrue == NULL)
      neut_tess_init_polytrue (&Tess);

    nem_stat (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, In, MeshPara, Tess);
  }

  // ###################################################################
  // ### CLOSING #######################################################
  
  nem_in_free (In);
  nem_meshpara_free (MeshPara);
  neut_tess_free  (&Tess);
  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh0D);
  neut_mesh_free (&Mesh1D);
  neut_mesh_free (&Mesh2D);
  neut_mesh_free (&Mesh3D);
  neut_nset_free (&NSet2D);
  neut_nset_free (&NSet1D);
  neut_nset_free (&NSet0D);
  neut_part_free (Part);
  neut_nodes_free (&RNodes);
  neut_mesh_free  (&RMesh0D);
  neut_mesh_free  (&RMesh1D);
  neut_mesh_free  (&RMesh2D);
  neut_mesh_free  (&RMesh3D);

  return EXIT_SUCCESS;
}
