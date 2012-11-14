/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFGeo.h"

int
GeoSearchEltPoly (struct GEO Geo, struct MESH* pMesh)
{
  int status = -1;
  int elt, pid, prevpid;
  char* progress = ut_alloc_1d_char (100);

  ut_print_progress (stdout, 0, (*pMesh).EltQty, "%3.0f%%", progress);
  prevpid = 1;
  for (elt = 1; elt <= (*pMesh).EltQty; elt++)
  {
    status = -1;

    if (neut_geo_point_inpoly (Geo, (*pMesh).EltCoo[elt], prevpid) == 1)
    {
      (*pMesh).EltElset[elt] = prevpid;
      status = 0;
    }
    else
    {
      for (pid = 1; pid <= Geo.PolyQty; pid++)
      {
	if (pid == prevpid)
	  continue;

	if (neut_geo_point_inpoly (Geo, (*pMesh).EltCoo[elt], pid) == 1)
	{
	  (*pMesh).EltElset[elt] = pid;
	  prevpid = pid;
	  status = 0;
	  break;
	}
      }

      if (status == -1 || (*pMesh).EltCoo[elt] == 0)
	ut_error_reportbug ();
    }

    /* print percent evolution in case of verbosity */
    ut_print_progress (stdout, elt, (*pMesh).EltQty, "%3.0f%%", progress);
  }

  ut_free_1d_char (progress);

  return status;
}
