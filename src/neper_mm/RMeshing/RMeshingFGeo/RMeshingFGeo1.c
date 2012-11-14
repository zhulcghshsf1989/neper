/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RMeshingFGeo.h"

void
RMeshingFGeo (struct GEO Geo, struct MESH* pMesh, struct GERMSET *pGermSet)
{
  int status;

  // this is for net_oin_fprintf_dneigh
  (*pGermSet).NDensity = Geo.PolyQty;
  (*pGermSet).N = Geo.PolyQty;
  (*pGermSet).Id = Geo.Id;

  /* Searching element poly */
  status = GeoSearchEltPoly (Geo, pMesh);
  if (status == -1)
  {
    ut_print_message (2, 0, "elt does not belong to any polyhedron.\n");
    ut_error_reportbug ();
  }
  
  /* Intializing Ramdom for oin */
  (*pGermSet).Random = neut_rand_nnid2rand (Geo.PolyQty, Geo.Id);

  return;
}
