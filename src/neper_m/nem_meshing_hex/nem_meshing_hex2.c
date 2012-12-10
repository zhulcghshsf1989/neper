/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_meshing_hex.h"

void
nem_meshing_hexFGeo (struct GEO Geo, struct MESH* pMesh)
{
  int elt, pid, prevpid;
  char* progress = ut_alloc_1d_char (100);

  ut_print_progress (stdout, 0, (*pMesh).EltQty, "%3.0f%%", progress);
  prevpid = 1;
  for (elt = 1; elt <= (*pMesh).EltQty; elt++)
  {
    if (neut_geo_point_inpoly (Geo, (*pMesh).EltCoo[elt], prevpid) == 1)
      (*pMesh).EltElset[elt] = prevpid;
    else
      for (pid = 1; pid <= Geo.PolyQty; pid++)
      {
	if (pid == prevpid)
	  continue;

	if (neut_geo_point_inpoly (Geo, (*pMesh).EltCoo[elt], pid) == 1)
	{
	  (*pMesh).EltElset[elt] = pid;
	  prevpid = pid;
	  break;
	}
      }

    /* print percent evolution in case of verbosity */
    ut_print_progress (stdout, elt, (*pMesh).EltQty, "%3.0f%%", progress);
  }

  ut_free_1d_char (progress);

  return;
}

void
nem_meshing_hex_meshpoly (char* meshpoly, struct VOX Vox, struct MESH* pMesh, struct NODES* pNodes,
                   struct NSET* pNSet2D)
{
  int i, polyqty;
  int* poly = NULL;

  neut_vox_expr_polys (Vox, meshpoly, &poly, &polyqty);

  for (i = 1; i <= Vox.PolyQty; i++)
    if (ut_array_1d_int_eltpos (poly, polyqty, i) == -1)
      neut_mesh_rmelset (pMesh, *pNodes, i);

  neut_nodes_rmorphans (pNodes, pMesh, pNSet2D);

  ut_free_1d_int (poly);

  return;
}
