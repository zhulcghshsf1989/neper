/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "nem_nsets.h"

void
nem_nsets (struct IN In, struct TESS Tess, struct MESH Mesh0D, struct MESH Mesh1D,
           struct MESH Mesh2D, struct NSET* pNSet0D, struct NSET* pNSet1D,
	   struct NSET* pNSet2D)
{
  if (In.nset == NULL || strlen (In.nset) == 0)
    return;

  if (! strcmp (In.elttype, "tet"))
  {
    nem_nsets_2d_tess (Tess, Mesh2D, pNSet2D);
    nem_nsets_1d_tess (Tess, Mesh1D, *pNSet2D, pNSet1D);
    nem_nsets_0d_tess (Tess, Mesh0D, *pNSet2D, pNSet0D);

    if (ut_string_finds (In.nset, "all") != -1
     || ut_string_finds (In.nset, "body") != -1
     || ut_string_finds (In.nset, "bodies") != -1)
    {
      nem_nsets_2dbody_tess (Tess, *pNSet1D, pNSet2D);
      nem_nsets_1dbody_tess (Tess, *pNSet0D, pNSet1D);
    }
  }

  else if (! strcmp (In.elttype, "hex"))
  {
    if (Tess.PolyQty > 0 && ! strcmp (Tess.DomType, "cube"))
    {
      nem_nsets_1d_tess_hex (Tess, *pNSet2D, pNSet1D);
      nem_nsets_0d_tess_hex (Tess, *pNSet2D, *pNSet1D, pNSet0D);

      if (ut_string_finds (In.nset, "all") != -1
       || ut_string_finds (In.nset, "body") != -1
       || ut_string_finds (In.nset, "bodies") != -1)
      {
	nem_nsets_2dbody_tess (Tess, *pNSet1D, pNSet2D);
	nem_nsets_1dbody_tess (Tess, *pNSet0D, pNSet1D);
      }
    }

    else
    {
      nem_nsets_1d_hex (*pNSet2D, pNSet1D);
      nem_nsets_0d_hex (*pNSet1D, pNSet0D);

      if (ut_string_finds (In.nset, "all") != -1
       || ut_string_finds (In.nset, "body") != -1
       || ut_string_finds (In.nset, "bodies") != -1)
      {
	nem_nsets_2dbody_hex (*pNSet1D, pNSet2D);
	nem_nsets_1dbody_hex (*pNSet0D, pNSet1D);
      }
    }
  }

  return;
}
