/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neper_mm.h"

int
neper_mm (int fargc, char **fargv, int argc, char **argv)
{
  struct IN In;
  struct GEO Geo;
  struct NODES Nodes;
  struct MESH Mesh0D;
  struct MESH Mesh1D;
  struct MESH Mesh2D;
  struct MESH Mesh3D;
  int **FoDNodes = NULL;

  neut_nodes_set_zero (&Nodes);
  neut_mesh_set_zero  (&Mesh3D);
  neut_mesh_set_zero  (&Mesh0D);
  neut_mesh_set_zero  (&Mesh1D);
  neut_mesh_set_zero  (&Mesh2D);
  neut_geo_set_zero   (&Geo);

  ut_print_moduleheader ("-MM", fargc, fargv, argc, argv);

  InputData_mm (&In, fargc, fargv, argc, argv);

  ut_print_message (0, 1, "Mapped meshing ...\n");
  
  if (In.loadmesh == NULL)
    RMeshing (&Nodes, &Mesh3D, &FoDNodes, In);
  else
  {
    ut_print_message (0, 1, "Loading mesh ...\n");
    ReadMesh (In.loadmesh, &Nodes, NULL, NULL, NULL, &Mesh3D);
    
    if (strcmp (Mesh3D.EltType, "quad") != 0)
    {
      ut_print_message (2, 0, "Input mesh is not a mapped mesh.\n");
      abort ();
    }
    SearchFoDNodes_cube (Nodes, &FoDNodes);
  }

  if (ut_string_inlist (In.outdim, ',', "2" )
   || ut_string_inlist (In.outdim, ',', "1" )
   || ut_string_inlist (In.outdim, ',', "0" ))
  {
    if (ut_string_inlist (In.format, ',', "tess")
     || ut_string_inlist (In.format, ',', "geo" ))
      ReconMesh ("3,2,1,0", Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, &Geo);
    else
      ReconMesh ("3,2,1,0", Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, NULL);
  }

  ut_print_message (0, 1, "Writing results ...\n");
  Res_mm (In, Geo, Nodes, &Mesh0D, &Mesh1D, &Mesh2D, &Mesh3D, FoDNodes);

  neut_nodes_free (&Nodes);
  neut_mesh_free (&Mesh0D);
  neut_mesh_free (&Mesh1D);
  neut_mesh_free (&Mesh2D);
  neut_mesh_free (&Mesh3D);

  return 0;
}
