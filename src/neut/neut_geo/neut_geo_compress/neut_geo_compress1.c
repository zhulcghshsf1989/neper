/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_compress_lcl.h"

/* This function deletes the dummy entities (ver, edge, face) in a Geo */
void
neut_geo_compress (struct GEO* pGeo)
{
  neut_geo_compress_faces (pGeo);
  neut_geo_compress_edges (pGeo);
  neut_geo_compress_vers (pGeo);

  return;
}
