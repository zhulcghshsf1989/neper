/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_init.h"

void
net_init_reg (struct TESS* pTess, struct REG* pReg)
{
  double vol;

  /* Calculation of sel */
  // seltype == 0: nothing to do
  if ((*pReg).seltype == 1)
  {
    neut_tess_volume (*pTess, &vol);
    rsel2sel ((*pReg).rsel, vol, (*pTess).PolyQty, &((*pReg).sel));
  }
  
  // Calculation of sel_skin
  if ((*pReg).dbound != NULL && (*pReg).dboundseltype == 1)
  {
    neut_tess_volume (*pTess, &vol);
    rsel2sel ((*pReg).dboundrsel, vol, (*pTess).PolyQty, &((*pReg).dboundsel));
  }

  return;
}
