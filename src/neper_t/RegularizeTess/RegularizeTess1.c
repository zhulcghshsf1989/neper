/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RegularizeTess.h"

void
RegularizeTess (struct TESS *pTess, struct TESSPARA TessPara,
	   struct IN In)
{
  int i, DelId = 0;

  (*pTess).maxff = TessPara.maxff;
  (*pTess).sel = TessPara.sel;
  if (TessPara.dbound != NULL)
  {
    (*pTess).dbound = ut_alloc_1d_char (strlen (TessPara.dbound) + 1);
    strcpy ((*pTess).dbound, TessPara.dbound);
    (*pTess).dboundsel = TessPara.dboundsel;
  }

  /*********************************************************************** 
   * if maxff > 0 & mloop >= 1, small edge deletion */

  if (TessPara.maxff > 1e-15 && In.mloop >= 1)
  {
    ut_print_message (0, 1, "Regularizing tessellation ... (sel = %.3g", TessPara.sel);
    if (TessPara.dbound)
      printf (", dboundsel = %.3g)\n", TessPara.dboundsel);
    else
      printf (")\n");

    ut_print_message (0, 2, "loop    length   deleted\n");
    for (i = 1; i <= In.mloop; i++)
      if (MergeTess (pTess, TessPara, In, &DelId, i) <= 0)
	break;
  }

  /*---------------------------------------------------------------------- 
   * compressing */
  neut_tess_compress (pTess);

  neut_tess_init_domtessface (pTess);
  neut_tess_init_domtessedge (pTess);
  neut_tess_init_domtessver  (pTess);
  
  /*********************************************************************** 
   * testing full tessellation */

  if (neut_tess_test (*pTess, 0) != 0 || neut_tess_test_dom (*pTess, 0) != 0)
  {
    ut_print_message (2, 0, "The tessellation is not valid.\n");
    abort ();
  }

  return;
}
