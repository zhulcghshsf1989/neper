/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets.h"

void
nem_nsets (struct IN In, struct GEO Geo, struct MESH Mesh0D, struct MESH Mesh1D,
           struct MESH Mesh2D, struct NSET* pNSet0D, struct NSET* pNSet1D,
	   struct NSET* pNSet2D)
{
  if (In.nset == NULL || strlen (In.nset) == 0)
    return;

  if (! strcmp (In.elttype, "tet"))
  {
    nem_nsets_2d_geo (Geo, Mesh2D, pNSet2D);
    nem_nsets_1d_geo (Geo, Mesh1D, *pNSet2D, pNSet1D);
    nem_nsets_0d_geo (Geo, Mesh0D, *pNSet2D, pNSet0D);

    if (ut_string_finds (In.nset, "all") != -1
     || ut_string_finds (In.nset, "body") != -1
     || ut_string_finds (In.nset, "bodies") != -1)
    {
      nem_nsets_2dbody_geo (Geo, *pNSet1D, pNSet2D);
      nem_nsets_1dbody_geo (Geo, *pNSet0D, pNSet1D);
    }
  }

  else if (! strcmp (In.elttype, "hex"))
  {
    if (Geo.PolyQty > 0 && ! strcmp (Geo.DomType, "cube"))
    {
      nem_nsets_1d_geo_hex (Geo, *pNSet2D, pNSet1D);
      nem_nsets_0d_geo_hex (Geo, *pNSet2D, *pNSet1D, pNSet0D);

      if (ut_string_finds (In.nset, "all") != -1
       || ut_string_finds (In.nset, "body") != -1
       || ut_string_finds (In.nset, "bodies") != -1)
      {
	nem_nsets_2dbody_geo (Geo, *pNSet1D, pNSet2D);
	nem_nsets_1dbody_geo (Geo, *pNSet0D, pNSet1D);
      }
    }
  }

  return;
}
