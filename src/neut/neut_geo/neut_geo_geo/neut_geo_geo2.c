/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_geo_geo_lcl.h"

void
neut_geo_geoGen (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoVerQty (GeoA, pGeoB);
  neut_geo_geoEdgeQty (GeoA, pGeoB);
  neut_geo_geoFaceQty (GeoA, pGeoB);
  neut_geo_geoPolyQty (GeoA, pGeoB);

  if (GeoA.version != NULL)
  {
    (*pGeoB).version = ut_alloc_1d_char (strlen (GeoA.version) + 1);
    strcpy ((*pGeoB).version, GeoA.version);
  }

  (*pGeoB).N = GeoA.N;
  (*pGeoB).Id = GeoA.Id;

  (*pGeoB).morpho = ut_alloc_1d_char (strlen (GeoA.morpho) + 1);
  strcpy ((*pGeoB).morpho, GeoA.morpho);

  (*pGeoB).Type = ut_alloc_1d_char (strlen (GeoA.Type) + 1);
  strcpy ((*pGeoB).Type, GeoA.Type);

  if (GeoA.PolyId != NULL)
  {
    (*pGeoB).PolyId = ut_alloc_1d_int (GeoA.PolyQty + 1);
    ut_array_1d_int_memcpy ((*pGeoB).PolyId + 1, GeoA.PolyQty,
	GeoA.PolyId + 1);
  }

  (*pGeoB).sel = GeoA.sel;
  (*pGeoB).maxff = GeoA.maxff;
  if (GeoA.dbound != NULL)
  {
    (*pGeoB).dbound = ut_alloc_1d_char (strlen (GeoA.dbound) + 1);
    strcpy ((*pGeoB).dbound, GeoA.dbound);
    (*pGeoB).dboundsel = GeoA.dboundsel;
  }

  return;
}

/* neut_geo_geotializing the properties of the vertices */
void
neut_geo_geoVer (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoVerCoo (GeoA, pGeoB);
  neut_geo_geoVerEdge (GeoA, pGeoB);
  neut_geo_geoVerBound (GeoA, pGeoB);
  neut_geo_geoVerState (GeoA, pGeoB);

  return;
}

/* neut_geo_geotializing the properties of the edges */
void
neut_geo_geoEdge (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoEdgeVerNb (GeoA, pGeoB);
  neut_geo_geoEdgeFace (GeoA, pGeoB);
  neut_geo_geoEdgeLength (GeoA, pGeoB);
  neut_geo_geoEdgeDel (GeoA, pGeoB);
  neut_geo_geoEdgeBound (GeoA, pGeoB);
  neut_geo_geoEdgeState (GeoA, pGeoB);

  return;
}

/* neut_geo_geotializing the properties of the faces */
void
neut_geo_geoFace (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoFacePoly (GeoA, pGeoB);
  neut_geo_geoFaceVer (GeoA, pGeoB);
  neut_geo_geoFaceEdge (GeoA, pGeoB);
  neut_geo_geoFaceFF (GeoA, pGeoB);
  neut_geo_geoFaceEq (GeoA, pGeoB);
  neut_geo_geoFaceBound (GeoA, pGeoB);
  neut_geo_geoFaceState (GeoA, pGeoB);
  neut_geo_geoFacePt (GeoA, pGeoB);

  return;
}

/* neut_geo_geotializing the properties of the polyhedra */
void
neut_geo_geoPoly (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoPolyFace (GeoA, pGeoB);
  neut_geo_geoPolyCenter (GeoA, pGeoB);
  neut_geo_geoPolyTrue (GeoA, pGeoB);
  neut_geo_geoPolyBody (GeoA, pGeoB);

  return;
}

/* neut_geo_geotializing the properties of the domain */
void
neut_geo_geoDomain (struct GEO GeoA, struct GEO *pGeoB)
{
  neut_geo_geoDomainData (GeoA, pGeoB);

  return;
}
