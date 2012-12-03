/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_init.h"

void
net_init_reg (struct GEO* pGeo, struct GEOPARA* pGeoPara)
{
  double vol;

  /* Calculation of sel */
  // seltype == 0: nothing to do
  if ((*pGeoPara).seltype == 1)
  {
    neut_geo_volume (*pGeo, &vol);
    rsel2sel ((*pGeoPara).rsel, vol, (*pGeo).PolyQty, &((*pGeoPara).sel));
  }
  
  // Calculation of sel_skin
  if ((*pGeoPara).dbound != NULL && (*pGeoPara).dboundseltype == 1)
  {
    neut_geo_volume (*pGeo, &vol);
    rsel2sel ((*pGeoPara).dboundrsel, vol, (*pGeo).PolyQty, &((*pGeoPara).dboundsel));
  }

  return;
}
