/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_regularization.h"

void
net_regularization (struct REG Reg, struct TESS *pTess)
{
  int DelId = 0;

  (*pTess).maxff = Reg.maxff;
  (*pTess).sel = Reg.sel;
  if (Reg.dbound != NULL)
  {
    (*pTess).dbound = ut_alloc_1d_char (strlen (Reg.dbound) + 1);
    strcpy ((*pTess).dbound, Reg.dbound);
    (*pTess).dboundsel = Reg.dboundsel;
  }

  /*********************************************************************** 
   * if maxff > 0 & mloop >= 1, small edge deletion */

  if (Reg.maxff > 1e-15 && Reg.mloop >= 1)
  {
    /*
    printf ("(sel = %.3g", Reg.sel);
    if (Reg.dbound)
      printf (", dboundsel = %.3g)", Reg.dboundsel);
    else
      printf (")");
    */

    MergeTess (pTess, Reg, &DelId);
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
