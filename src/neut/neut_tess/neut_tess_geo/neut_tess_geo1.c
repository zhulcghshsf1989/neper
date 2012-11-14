/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geo.h"

void
neut_tess_geo (struct TESS Tess, struct GEO *pGeo)
{
  neut_geo_free (pGeo);
  
  neut_tess_geo_general  (Tess, pGeo);
  neut_tess_geo_ver      (Tess, pGeo);
  neut_tess_geo_edge     (Tess, pGeo);
  neut_tess_geo_face     (Tess, pGeo);
  neut_tess_geo_poly     (Tess, pGeo);
  
  return;
}
