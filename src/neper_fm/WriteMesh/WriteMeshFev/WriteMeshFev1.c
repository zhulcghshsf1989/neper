/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshFev.h"

void
WriteMeshFev (struct IN In, struct GEO Geo,
	      struct NODES Nodes, struct MESH Mesh2D, struct MESH Mesh3D,
	      struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
	      char *nset)
{
  FILE *file;
  char* bcs = NULL;
  char* surf = NULL;
  char* fasetexpand = NULL;

  if (strcmp (Geo.DomType, "cube"))
  {
    ut_print_message (1, 0, "Fev output available for a cuboidal domain only.");
    return;
  }

  neut_nset_expand (NSet0D, NSet1D, NSet2D, In.faset, &fasetexpand);

  /* fev1, parms = general parameters */
  file = ut_file_open (In.fev1, "w");
  neut_mesh_fprintf_fev_parms (file, Nodes, Mesh3D);
  ut_file_close (file, In.fev1, "w");

  /* fev2, mesh = connectivity and geometry */
  file = ut_file_open (In.fev2, "w");
  neut_mesh_fprintf_fev_elts (file, Geo, Mesh3D);
  fprintf (file, "  1.0   1.0   1.0\n");
  neut_mesh_fprintf_fev_nodes (file, Nodes);
  ut_file_close (file, In.fev2, "w");

  /* fev3, surf = connectivity at the surface (2D elements) */
  file = ut_file_open (In.fev3, "w");
  neut_mesh_fprintf_fev_skinelts (file, Geo, Mesh2D, Mesh3D, Nodes, fasetexpand);
  ut_file_close (file, In.fev3, "w");

  /* fev4, opt = element sets */
  file = ut_file_open (In.fev4, "w");
  neut_mesh_fprintf_fev_elsets (file, Mesh3D);
  ut_file_close (file, In.fev4, "w");

  /* fev5, bcs = node sets */
  file = ut_file_open (In.fev5, "w");
  neut_mesh_fprintf_fev_nsets (file, NSet0D, NSet1D, NSet2D, nset);
  ut_file_close (file, In.fev5, "w");

  ut_free_1d_char (bcs);
  ut_free_1d_char (surf);
  ut_free_1d_char (fasetexpand);

  return;
}

