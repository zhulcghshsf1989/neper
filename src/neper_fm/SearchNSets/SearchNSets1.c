/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "SearchNSets.h"

void
SearchNSets (char* nsets, struct GEO Geo, struct MESH Mesh0D, struct MESH Mesh1D,
             struct MESH Mesh2D, struct NSET* pNSet0D, struct NSET* pNSet1D,
	     struct NSET* pNSet2D)
{
  if (nsets == NULL || strlen (nsets) == 0)
    return;

  SearchNSets_2d (Geo, Mesh2D, pNSet2D);

  SearchNSets_1d (Geo, Mesh1D, *pNSet2D, pNSet1D);
  
  SearchNSets_0d (Geo, Mesh0D, *pNSet2D, pNSet0D);

  if (ut_string_finds (nsets, "all") != -1
   || ut_string_finds (nsets, "body") != -1
   || ut_string_finds (nsets, "bodies") != -1)
  {
    SearchNSets_2d_body (Geo, *pNSet1D, pNSet2D);
    SearchNSets_1d_body (Geo, *pNSet0D, pNSet1D);
  }

  return;
}
