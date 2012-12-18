/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_init.h"

void
net_init_reg (struct TESS* pTess, struct TESSPARA* pTessPara)
{
  double vol;

  /* Calculation of sel */
  // seltype == 0: nothing to do
  if ((*pTessPara).seltype == 1)
  {
    neut_tess_volume (*pTess, &vol);
    rsel2sel ((*pTessPara).rsel, vol, (*pTess).PolyQty, &((*pTessPara).sel));
  }
  
  // Calculation of sel_skin
  if ((*pTessPara).dbound != NULL && (*pTessPara).dboundseltype == 1)
  {
    neut_tess_volume (*pTess, &vol);
    rsel2sel ((*pTessPara).dboundrsel, vol, (*pTess).PolyQty, &((*pTessPara).dboundsel));
  }

  return;
}
