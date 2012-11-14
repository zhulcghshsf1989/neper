/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

/* This file is ext of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neper_fm.h"

int
neper_fm (int fargc, char **fargv, int argc, char **argv)
{
  int i;
  struct IN In;
  struct GEO Geo;
  struct GEOPARA GeoPara;
  struct NODES Nodes;
  struct MESH Mesh0D, Mesh1D, Mesh2D, Mesh3D;
  struct PART Part;
  struct NSET NSet0D, NSet1D, NSet2D;
  FILE* file = NULL;

  /* initializations */
  nefm_in_set_zero    (&In);
  nefm_geopara_set_zero (&GeoPara);
  neut_geo_set_zero   (&Geo);
  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero  (&Mesh0D);
  neut_mesh_set_zero  (&Mesh1D);
  neut_mesh_set_zero  (&Mesh2D);
  neut_mesh_set_zero  (&Mesh3D);
  neut_part_set_zero  (&Part);
  neut_nset_set_zero  (&NSet0D);
  neut_nset_set_zero  (&NSet1D);
  neut_nset_set_zero  (&NSet2D);

  /* Printing program header */
  ut_print_moduleheader ("-FM", fargc, fargv, argc, argv);

  /* Reading input data */
  ut_print_message (0, 1, "Reading input data ...\n");
  InputData_fm (&In, &GeoPara, fargc, fargv, argc, argv);

  neut_gmsh_rc ("bak");

/*********************************************************************** 
 * Creating or importing geometry */

  if (In.ingeo != NULL)
  {
    ut_print_message (0, 1, "Creating geometry ...\n");
    nefm_init_geo (In.ingeo, &Geo, &GeoPara);
    // RegularizeGeo (&Geo, GeoPara, In);
  }

/*********************************************************************** 
 * Meshing (or loading mesh) */
  
  if (In.mesh == 1)
  {
    if (GeoPara.dboundcl < 0)
      ut_print_message (0, 1, "Meshing ... (cl = %.3g, pl = %.3g)\n",
	  GeoPara.cl, GeoPara.pcl);
    else
    {
      ut_print_message (0, 1, "Meshing ... (cl = %.3g, pl = %.3g,\n", GeoPara.cl, GeoPara.pcl);
      ut_print_message (0, 1, "             dboundcl = %.3g, dboundpl = %.3g)\n", GeoPara.dboundcl, GeoPara.dboundpcl);
    }

    double** face_eq = ut_alloc_2d (Geo.FaceQty + 1, 4);
    int* face_sim = ut_alloc_1d_int (Geo.FaceQty + 1);
    double*** face_sim_A = ut_alloc_3d (Geo.FaceQty + 1, 3, 3);
    double**  face_sim_B = ut_alloc_2d (Geo.FaceQty + 1, 3);

    if (In.meshdim >= 0)
      Premeshing (Geo, NULL, NULL, NULL,
	  face_sim, face_sim_A, face_sim_B, face_eq, NULL, NULL);

    if (In.meshdim >= 0)
      Meshing0D (Geo, GeoPara, &Nodes, &Mesh0D);

    if (In.meshdim >= 1)
      Meshing1D (Geo, GeoPara, NULL, NULL, NULL, &Nodes, &Mesh1D);

    if (In.meshdim >= 2)
      Meshing2D (In, GeoPara, Geo, face_sim, face_sim_A, face_sim_B,
	  face_eq, NULL, NULL, NULL, Mesh0D, Mesh1D, &Nodes, &Mesh2D);

    ut_free_2d (face_eq, Geo.FaceQty + 1);
    ut_free_1d_int (face_sim);
    ut_free_3d (face_sim_A, Geo.FaceQty + 1, 3);
    ut_free_2d (face_sim_B, Geo.FaceQty + 1);

    if (In.meshdim >= 3)
      Meshing3D (In, GeoPara, Geo, &Nodes, Mesh2D, &Mesh3D);

    if (GeoPara.cltype == 2)
      neut_nodes_scale (&Nodes, GeoPara.cl3[0] / GeoPara.cl, GeoPara.cl3[1] / GeoPara.cl, GeoPara.cl3[2] / GeoPara.cl);
    if (GeoPara.cltype == 3)
      neut_nodes_scale (&Nodes, GeoPara.rcl3[0] / GeoPara.rcl, GeoPara.rcl3[1] / GeoPara.rcl, GeoPara.rcl3[2] / GeoPara.rcl);
  }
  else if (In.loadmesh != NULL)
  {
    ut_print_message (0, 1, "Loading mesh ...\n");
    ReadMesh (In.loadmesh, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D);
  }

  // remeshing
  else if (In.remesh)
  {
    if (strcmp (Mesh3D.EltType, "tri") != 0)
    {
      ut_print_message (2, 0, "Input mesh is not a free mesh.\n");
      printf ("elttype = %s\n", Mesh3D.EltType);
      abort ();
    }

    struct NODES RNodes;
    struct MESH RMesh0D, RMesh1D, RMesh2D, RMesh3D;

    neut_nodes_set_zero (&RNodes);
    neut_mesh_set_zero  (&RMesh0D);
    neut_mesh_set_zero  (&RMesh1D);
    neut_mesh_set_zero  (&RMesh2D);
    neut_mesh_set_zero  (&RMesh3D);

    ut_print_message (0, 1, "Remeshing ...\n");
    nefm_init_remesh (In, &Geo, &RNodes, &RMesh0D, &RMesh1D, &RMesh2D,
	&RMesh3D, &GeoPara);

    Remeshing (In, GeoPara, &Geo, &RNodes, RMesh0D, &RMesh1D, &RMesh2D,
	RMesh3D, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D);

    if (GeoPara.cltype == 2)
      neut_nodes_scale (&Nodes, GeoPara.cl3[0] / GeoPara.cl, GeoPara.cl3[1] / GeoPara.cl, GeoPara.cl3[2] / GeoPara.cl);
    if (GeoPara.cltype == 3)
      neut_nodes_scale (&Nodes, GeoPara.rcl3[0] / GeoPara.rcl, GeoPara.rcl3[1] / GeoPara.rcl, GeoPara.rcl3[2] / GeoPara.rcl);

    neut_nodes_free (&RNodes);
    neut_mesh_free  (&RMesh0D);
    neut_mesh_free  (&RMesh1D);
    neut_mesh_free  (&RMesh2D);
    neut_mesh_free  (&RMesh3D);
  }

  if (In.nodecoo != NULL)
  {
    file = ut_file_open (In.nodecoo, "r");
    for (i = 1; i <= Nodes.NodeQty; i++)
      ut_array_1d_fscanf (file, Nodes.NodeCoo[i], 3);
    ut_file_close (file, In.nodecoo, "r");
  }

  // if (strcmp (In.printmeshentity, "none") != 0 && Geo.VerQty == 0)
  if (Mesh3D.EltQty != 0 && (Mesh2D.EltQty == 0 || Mesh1D.EltQty == 0 ||
      Mesh0D.EltQty == 0 || Geo.VerQty == 0))
  {
    ut_print_message (0, 1, "Reconstructing mesh ...\n");

    neut_nodes_init_boundingbox (&Nodes);
    ReconMesh (In.outdim, Nodes, &Mesh0D, &Mesh1D, &Mesh2D,
	       &Mesh3D, &Geo);
  }

  if (Mesh3D.EltQty > 0)
    if (Mesh3D.EltOrder == 1 && In.morder == 2)
    {
      ut_print_message (0, 2, "Switching mesh to order 2 ...\n");
      fflush (stdout);
      neut_mesh_order1to2 (&Nodes, &Mesh1D, &Mesh2D, &Mesh3D);
    }

  if (strlen (In.nset) > 0 && Mesh3D.EltQty > 0)
  {
    ut_print_message (0, 2, "Searching nsets ...\n");
    fflush (stdout);

    SearchNSets (In.nset, Geo, Mesh0D, Mesh1D, Mesh2D, &NSet0D, &NSet1D, &NSet2D);
  }

/*********************************************************************** 
 * mesh output */

  // Partition
#ifdef HAVE_LIBSCOTCH
  if (In.partmode != -1)
  {
    ut_print_message (0, 1, "Partitioning mesh ...\n");
    fflush (stdout);
    Partition (In, &Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, &NSet0D, &NSet1D, &NSet2D, &Part);
  }
#endif

  // Write mesh
  if (In.mesh || In.remesh || In.loadmesh != NULL)
  {
    ut_print_message (0, 1, "Writing mesh results ...\n");
    fflush (stdout);

    WriteMesh (In, Geo, Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, NSet0D, NSet1D, NSet2D, Part);
  }

  neut_nset_free  (&NSet2D);
  neut_nset_free  (&NSet1D);
  neut_nset_free  (&NSet0D);
  neut_part_free  (Part);
  neut_gmsh_rc ("unbak");

/*********************************************************************** 
 * Remapping */
  if (In.remap > 0)
  {
    struct NODES RNodes;
    struct MESH RMesh0D, RMesh1D, RMesh2D, RMesh3D;
    neut_nodes_set_zero (&RNodes);
    neut_mesh_set_zero  (&RMesh0D);
    neut_mesh_set_zero  (&RMesh1D);
    neut_mesh_set_zero  (&RMesh2D);
    neut_mesh_set_zero  (&RMesh3D);

    ut_print_message (0, 1, "Mesh data transport ...\n");
    fflush (stdout);
    nefm_init_remap (In, &Geo, &RNodes, &RMesh0D, &RMesh1D, &RMesh2D, &RMesh3D);
    Remapping (In, Geo, RNodes, RMesh2D, RMesh3D, &Nodes, &Mesh2D, &Mesh3D);

    neut_nodes_free (&RNodes);
    neut_mesh_free  (&RMesh0D);
    neut_mesh_free  (&RMesh1D);
    neut_mesh_free  (&RMesh2D);
    neut_mesh_free  (&RMesh3D);
  }

/*********************************************************************** 
 * mesh output (statistics) */
  
  if (In.stn != NULL || In.ste != NULL || In.stelset != NULL)
  {
    ut_print_message (0, 1, "Writing mesh statistics ...\n");
    if (Geo.PolyTrue == NULL)
      neut_geo_init_polytrue (&Geo);

    nem_stat (Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D, In, GeoPara, &Geo);
  }

  // TODO init and free NSET
  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh0D);
  neut_mesh_free (&Mesh1D);
  neut_mesh_free (&Mesh2D);
  neut_mesh_free (&Mesh3D);
  neut_geo_free (&Geo);

  nefm_in_free (In);
  nefm_geopara_free (GeoPara);

  remove ("debugp.pos");
  remove ("debugr.pos");
  remove ("tmp.geo");
  remove ("tmp.msh");

  return 0;
}
