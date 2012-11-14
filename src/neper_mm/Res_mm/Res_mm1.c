/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "Res_mm.h"

void
Res_mm (struct IN In, struct GEO Geo, struct NODES Nodes,
        struct MESH* pMesh0D, struct MESH* pMesh1D,
	struct MESH* pMesh2D, struct MESH* pMesh3D,
        int** FoDNodes)
{
  int *PolyEltQty = NULL;
  /* [0]: real nb of polys (ie that have elements) */
  /* [i]: nb of elts of poly i */

  FILE *file = NULL;
  char *expandnset = NULL;
  char *expandfaset = NULL;

  // CLEAN by replacing neut_utils_nset_expand by neut_set_expand
  neut_utils_nset_expand (In.nset, &expandnset);
  neut_utils_nset_expand (In.faset, &expandfaset);

  CalcPolyEltQty ((*pMesh3D), &PolyEltQty);
  
  if (ut_string_inlist (In.format, ',', "tess")
   || ut_string_inlist (In.format, ',', "geo"))
    ut_print_message (0, 2, "Writing tessellation results ...\n");

  if (ut_string_inlist (In.format, ',', "tess"))
  {
    file = ut_file_open (In.geo, "w");
    neut_geo_fprintf (file, Geo);
    ut_file_close (file, In.geo, "w");
  }
  
  if (ut_string_inlist (In.format, ',', "geo"))
  {
    file = ut_file_open (In.gmshgeo, "w");
    neut_geo_fprintf_gmsh (file, Geo);
    ut_file_close (file, In.gmshgeo, "w");
  }
  
  if (ut_string_inlist (In.format, ',', "msh")
   || ut_string_inlist (In.format, ',', "geof")
   || ut_string_inlist (In.format, ',', "abq")
   || ut_string_inlist (In.format, ',', "inp"))
    ut_print_message (0, 2, "Writing mesh results ...\n");

  if (ut_string_inlist (In.format, ',', "msh"))
  {
    file = ut_file_open (In.msh, "w");
    WriteMapMeshGmsh (In.outdim, Nodes, pMesh0D, pMesh1D, pMesh2D,
	              pMesh3D, file);
    ut_file_close (file, In.msh, "w");
  }

  if (ut_string_inlist (In.format, ',', "geof"))
  {
    file = ut_file_open (In.geof, "w");
    WriteMapMeshGeof (In, expandnset, expandfaset,
		   Nodes, (*pMesh3D), FoDNodes, file);
    ut_file_close (file, In.geof, "w");
  }
  
  if (ut_string_inlist (In.format, ',', "abq")
   || ut_string_inlist (In.format, ',', "inp"))
  {
    file = ut_file_open (In.abq, "w");
    WriteMapMeshAbq (In, expandnset, expandfaset,
		   Nodes, (*pMesh3D), FoDNodes, file);
    ut_file_close (file, In.abq, "w");
  }

  ut_free_1d_int (PolyEltQty);

  return;
}
