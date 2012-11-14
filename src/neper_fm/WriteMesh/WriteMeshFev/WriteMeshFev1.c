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
  FILE* file = NULL;
  int printall, printparms, printmesh, printsurf, printopt, printbcs;

  printall = printparms = printmesh = printsurf = printopt = printbcs = 0;

  if (ut_string_inlist (In.format, ',', "fev"))
    printall = 1;
  else
  {
    printparms = ut_string_inlist (In.format, ',', "parms");
    printmesh  = ut_string_inlist (In.format, ',', "mesh");
    printsurf  = ut_string_inlist (In.format, ',', "surf");
    printopt   = ut_string_inlist (In.format, ',', "opt");
    printbcs   = ut_string_inlist (In.format, ',', "bcs");
  }

  /* fev1, parms = general parameters */
  if (printall || printparms)
  {
    file = ut_file_open (In.fev1, "w");
    neut_mesh_fprintf_fev_parms (file, Nodes, Mesh3D);
    ut_file_close (file, In.fev1, "w");
  }

  /* fev2, mesh = connectivity and geometry */
  if (printall || printmesh)
  {
    file = ut_file_open (In.fev2, "w");
    neut_mesh_fprintf_fev_elts (file, Geo, NSet2D, Mesh3D);
    fprintf (file, "  1.0   1.0   1.0\n");
    neut_mesh_fprintf_fev_nodes (file, Nodes);
    ut_file_close (file, In.fev2, "w");
  }

  /* fev3, surf = connectivity at the surface (2D elements) */
  if (printall || printsurf)
  {
    char* fasetexpand = NULL;
    neut_nset_expand (NSet0D, NSet1D, NSet2D, In.faset, &fasetexpand);
    file = ut_file_open (In.fev3, "w");
    neut_mesh_fprintf_fev_skinelts (file, Geo, Mesh2D, Mesh3D, Nodes,
	NSet2D, fasetexpand);
    ut_file_close (file, In.fev3, "w");
    ut_free_1d_char (fasetexpand);
  }

  /* fev4, opt = element sets */
  if (printall || printopt)
  {
    file = ut_file_open (In.fev4, "w");
    neut_mesh_fprintf_fev_elsets (file, Mesh3D);
    ut_file_close (file, In.fev4, "w");
  }

  /* fev5, bcs = node sets */
  if (printall || printbcs)
  {
    file = ut_file_open (In.fev5, "w");
    neut_mesh_fprintf_fev_nsets (file, NSet0D, NSet1D, NSet2D, nset);
    ut_file_close (file, In.fev5, "w");
  }

  return;
}
