/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_stat.h"

void
nem_stat (struct NODES Nodes,
	  struct MESH Mesh0D, struct MESH Mesh1D,
	  struct MESH Mesh2D, struct MESH Mesh3D, 
	  struct IN In, struct TESSPARA TessPara, struct TESS Tess)
{
  FILE* file = NULL;
  char* name = NULL;

  if (In.stn != NULL)
  {
    name = ut_string_addextension (In.body, "stnode");

    file = ut_file_open (name, "w");
    nem_stat_nodes (file, In.stn, Nodes, Mesh0D, Mesh1D, Mesh2D, Mesh3D);
    ut_file_close (file, name, "w");

    ut_free_1d_char (name);
  }

  if (In.ste != NULL)
  {
    name = ut_string_addextension (In.body, "stelt");

    file = ut_file_open (name, "w");
    nem_stat_elts (file, In.ste, Nodes, Mesh3D, Tess);
    ut_file_close (file, name, "w");

    ut_free_1d_char (name);
  }

  if (In.stelset != NULL)
  {
    name = ut_string_addextension (In.body, "stelset");

    file = ut_file_open (name, "w");
    nem_stat_elsets (file, In.stelset, Nodes, Mesh3D, TessPara, Tess);
    ut_file_close (file, name, "w");

    ut_free_1d_char (name);
  }

  return;
}
