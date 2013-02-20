/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_readmesh.h"

void
nem_readmesh (char* filename, char* filename2, struct NODES *pNodes,
          struct MESH *pMesh0D, struct MESH *pMesh1D,
	  struct MESH *pMesh2D, struct MESH *pMesh3D)
{
  int i;
  FILE *file = NULL, *file2 = NULL, *file3 = NULL;
  char *parms = NULL, *mesh = NULL, *opt = NULL;
  char *stenode = NULL, *steele = NULL;
  // int** Elsets = NULL;
  // int ElsetQty;
  char* filetype = NULL;
  
  ut_file_format (filename, &filetype);

  if (strcmp (filetype, "gmsh_msh") == 0)
  {
    file = ut_file_open (filename, "r");
    neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, pMesh3D);
    ut_file_close (file, filename, "r");
  }
  else if (strcmp (filetype, "zebulon_geof") == 0)
  {
    file = ut_file_open (filename, "r");
    neut_mesh_fscanf_geof (file, pNodes, pMesh3D);
    ut_file_close (file, filename, "r");
  }
  else if (strcmp (filetype, "fepx") == 0)
  {
    /* initializing file names */
    if (ut_file_exist ("%s.fepx1", filename))
      parms = ut_string_paste (filename, ".fepx1");
    else if (ut_file_exist ("%s.parms", filename))
      parms = ut_string_paste (filename, ".parms");

    if (parms == NULL)
    {
      ut_print_message (2, 2, "no .fepx1 or .parms file found.\n");
      abort ();
    }

    if (ut_file_exist ("%s.fepx2", filename))
      mesh = ut_string_paste (filename, ".fepx2");
    else if (ut_file_exist ("%s.mesh", filename))
      mesh = ut_string_paste (filename, ".mesh");

    if (mesh == NULL)
    {
      ut_print_message (2, 2, "no .fepx2 or .mesh file found.\n");
      abort ();
    }

    if (ut_file_exist ("%s.fepx4", filename))
      opt = ut_string_paste (filename, ".fepx4");
    else if (ut_file_exist ("%s.opt", filename))
      opt = ut_string_paste (filename, ".opt");

    if (opt == NULL)
    {
      ut_print_message (2, 2, "no .fepx4 or .opt file found.\n");
      abort ();
    }

    file = ut_file_open (parms, "r");
    file2 = ut_file_open (mesh, "r");
    file3 = ut_file_open (opt, "r");
    neut_mesh_fscanf_fepx (file, file2, file3, pNodes, pMesh3D);
    ut_file_close (file, parms, "r");
    ut_file_close (file2, mesh, "r");
    ut_file_close (file3, opt, "r");

    ut_free_1d_char (parms);
    ut_free_1d_char (mesh);
    ut_free_1d_char (opt);

    neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);
    // neut_mesh3d_mesh2d (*pNodes, *pMesh3D, pMesh2D, &Elsets, &ElsetQty);
  }
  else if (strcmp (filetype, "stellar") == 0)
  {
    stenode = ut_alloc_1d_char (strlen (filename) + 8);
    stenode = ut_string_addextension (filename, ".node");
    file = ut_file_open (stenode, "r");

    steele = ut_alloc_1d_char (strlen (filename) + 8);
    steele = ut_string_addextension (filename, ".ele");
    file2 = ut_file_open (steele, "r");

    neut_mesh_fscanf_stellar (file, file2, pNodes, pMesh3D);

    ut_file_close (file, stenode, "r");
    ut_file_close (file2, steele, "r");
  }

  neut_mesh_init_nodeelts (pMesh3D, (*pNodes).NodeQty);
  // should not be NodeQty below (should be smaller (RAM))
  neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);
  neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);
  neut_mesh_init_nodeelts (pMesh0D, (*pNodes).NodeQty);

  if (filename2 != NULL)
  {
    file = ut_file_open (filename2, "r");
    for (i = 1; i <= (*pNodes).NodeQty; i++)
      ut_array_1d_fscanf (file, (*pNodes).NodeCoo[i], 3);
    ut_file_close (file, filename2, "r");
  }

  ut_free_1d_char (filetype);

  return;
}