/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_geo_lcl.h"

void
neut_geo_geo (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_free (pGeoB);

  neut_geo_geoGen  (GeoA, pGeoB);
  neut_geo_geoVer  (GeoA, pGeoB);
  neut_geo_geoEdge (GeoA, pGeoB);
  neut_geo_geoFace (GeoA, pGeoB);
  neut_geo_geoPoly (GeoA, pGeoB);
  neut_geo_geoDomain (GeoA, pGeoB);

  return;
}
