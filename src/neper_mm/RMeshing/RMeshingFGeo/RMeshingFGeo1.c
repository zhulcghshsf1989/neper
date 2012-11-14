/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFGeo.h"

void
RMeshingFGeo (struct GEO Geo, struct MESH* pMesh)
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
