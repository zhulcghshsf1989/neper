/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RegularizeGeo.h"

void
RegularizeGeo (struct GEO *pGeo, struct GEOPARA GeoPara,
	   struct IN In)
{
  int i, DelId = 0;

  (*pGeo).maxff = GeoPara.maxff;
  (*pGeo).sel = GeoPara.sel;
  if (GeoPara.dbound != NULL)
  {
    (*pGeo).dbound = ut_alloc_1d_char (strlen (GeoPara.dbound) + 1);
    strcpy ((*pGeo).dbound, GeoPara.dbound);
    (*pGeo).dboundsel = GeoPara.dboundsel;
  }

  /*********************************************************************** 
   * if maxff > 0 & mloop >= 1, small edge deletion */

  if (GeoPara.maxff > 1e-15 && In.mloop >= 1)
  {
    ut_print_message (0, 1, "Regularizing tessellation ... (sel = %.3g", GeoPara.sel);
    if (GeoPara.dbound)
      printf (", dboundsel = %.3g)\n", GeoPara.dboundsel);
    else
      printf (")\n");

    ut_print_message (0, 2, "loop    length   deleted\n");
    for (i = 1; i <= In.mloop; i++)
      if (MergeGeo (pGeo, GeoPara, In, &DelId, i) <= 0)
	break;
  }

  /*---------------------------------------------------------------------- 
   * compressing */
  neut_geo_compress (pGeo);

  neut_geo_init_domtessface (pGeo);
  neut_geo_init_domtessedge (pGeo);
  neut_geo_init_domtessver  (pGeo);
  
  /*********************************************************************** 
   * testing full geometry */

  if (neut_geo_test (*pGeo, 0) != 0 || neut_geo_test_dom (*pGeo, 0) != 0)
  {
    ut_print_message (2, 0, "The tessellation is not valid.\n");
    abort ();
  }

  return;
}
