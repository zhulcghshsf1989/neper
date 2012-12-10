/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_load.h"

void
nevs_load (char** argv, int* pi, struct NODES *pNodes,
	   struct MESH *pMesh0D, struct MESH *pMesh1D,
	   struct MESH *pMesh2D, struct MESH *pMesh3D,
	   struct GEO *pGeo)
{
  FILE* file = NULL;
  
  // load tessellation -------------------------------------------------
  if (strcmp (argv[*pi], "-loadtess") == 0)
  {
    ut_print_message (0, 1, "Loading tessellation ...\n");
    file = ut_file_open (argv[++(*pi)], "r");
    neut_geo_fscanf (file, pGeo);
    ut_file_close (file, argv[(*pi)], "r");
  }

  // load mesh ---------------------------------------------------------
  else if (strcmp (argv[*pi], "-loadmesh") == 0)
  {
    ut_print_message (0, 1, "Loading mesh ...\n");
    file = ut_file_open (argv[++(*pi)], "r");
    neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D);
    ut_file_close (file, argv[(*pi)], "r");

    ut_print_message (0, 1, "Reconstructing mesh ...\n");

    // WORKAROUND for the case of elttype="quad" for which Geo is not
    // reconstructed properly.
    nem_reconmesh ("3,2,1,0", pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D, pGeo);
  }

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
