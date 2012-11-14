/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshGeof.h"

void
WriteMeshGeof (FILE* file, struct NODES Nodes, struct MESH Mesh2D,
	       struct MESH Mesh3D, struct NSET NSet0D, struct NSET NSet1D,
	       struct NSET NSet2D, char* nsets,
	       int liset, int part, struct PART Part)
{
  liset = liset;
  Mesh2D.EltQty = Mesh2D.EltQty;

  neut_mesh_fprintf_geof_head (file);

  neut_mesh_fprintf_geof_nodes (file, Nodes);

  neut_mesh_fprintf_geof_elts (file, Mesh3D);
  
  neut_mesh_fprintf_geof_sethead (file);

  neut_mesh_fprintf_geof_nsets (file, NSet0D, NSet1D, NSet2D, nsets);

  /*
  if (liset == 1 && (Geo.VerQty == 0 || !FoD))
    ut_print_message (1, 3, "Cannot output lisets (input is not a Geo).\n");
  else if (liset == 1)
    neut_mesh_fprintf_geof_lisets (Geo, FoD, Nodes, Mesh2D, file);
    */

  neut_mesh_fprintf_geof_elsets (file, Mesh3D);

  if (part == 1)
    neut_mesh_fprintf_geof_part (file, Part);

  neut_mesh_fprintf_geof_foot (file);

  return;
}
