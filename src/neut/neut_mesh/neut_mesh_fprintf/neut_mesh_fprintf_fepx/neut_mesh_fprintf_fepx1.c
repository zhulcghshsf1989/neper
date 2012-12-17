/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_fepx_lcl.h"

void
neut_mesh_fprintf_fepx_name (char* format, char* body, struct GEO Geo,
	      struct NODES Nodes, struct MESH Mesh2D, struct MESH Mesh3D,
	      struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D,
	      char *nset, char* faset)
{
  FILE* file = NULL;
  int printall, printparms, printmesh, printsurf, printopt, printbcs;
  char *parms, *mesh, *surf, *opt, *bcs;

  printall = printparms = printmesh = printsurf = printopt = printbcs = 0;

  parms = ut_string_addextension (body, ".parms");
  mesh  = ut_string_addextension (body, ".mesh");
  surf  = ut_string_addextension (body, ".surf");
  opt   = ut_string_addextension (body, ".opt");
  bcs   = ut_string_addextension (body, ".bcs");

  if (ut_string_inlist (format, ',', "fepx"))
    printall = 1;
  else
  {
    printparms = ut_string_inlist (format, ',', "parms");
    printmesh  = ut_string_inlist (format, ',', "mesh");
    printsurf  = ut_string_inlist (format, ',', "surf");
    printopt   = ut_string_inlist (format, ',', "opt");
    printbcs   = ut_string_inlist (format, ',', "bcs");
  }

  /* fepx1, parms = general parameters */
  if (printall || printparms)
  {
    file = ut_file_open (parms, "w");
    neut_mesh_fprintf_fepx_parms (file, Nodes, Mesh3D);
    ut_file_close (file, parms, "w");
  }

  /* fepx2, mesh = connectivity and geometry */
  if (printall || printmesh)
  {
    file = ut_file_open (mesh, "w");
    neut_mesh_fprintf_fepx_elts (file, Geo, NSet2D, Mesh3D);
    fprintf (file, "  1.0   1.0   1.0\n");
    neut_mesh_fprintf_fepx_nodes (file, Nodes);
    ut_file_close (file, mesh, "w");
  }

  /* fepx3, surf = connectivity at the surface (2D elements) */
  if (printall || printsurf)
  {
    char* fasetexpand = NULL;
    neut_nset_expand (NSet0D, NSet1D, NSet2D, faset, &fasetexpand);
    file = ut_file_open (surf, "w");
    neut_mesh_fprintf_fepx_skinelts (file, Geo, Mesh2D, Mesh3D, Nodes,
	NSet2D, fasetexpand);
    ut_file_close (file, surf, "w");
    ut_free_1d_char (fasetexpand);
  }

  /* fepx4, opt = element sets */
  if (printall || printopt)
  {
    file = ut_file_open (opt, "w");
    neut_mesh_fprintf_fepx_elsets (file, Mesh3D);
    ut_file_close (file, opt, "w");
  }

  /* fepx5, bcs = node sets */
  if (printall || printbcs)
  {
    file = ut_file_open (bcs, "w");
    neut_mesh_fprintf_fepx_nsets (file, NSet0D, NSet1D, NSet2D, nset);
    ut_file_close (file, bcs, "w");
  }

  return;
}
