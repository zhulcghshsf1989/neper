/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_tess_geo.h"

void
neut_tess_geo_general (struct TESS Tess, struct GEO* pGeo)
{
  (*pGeo).N  = Tess.PolyQty;

  (*pGeo).Id = Tess.Id;

  (*pGeo).Type = ut_alloc_1d_char (strlen (Tess.Type) + 1);
  strcpy ((*pGeo).Type, Tess.Type);

  (*pGeo).morpho = ut_alloc_1d_char (strlen (Tess.morpho) + 1);
  strcpy ((*pGeo).morpho, Tess.morpho); 

  return;
}

/* neut_tess_geotializing the properties of the vertices */
void
neut_tess_geo_ver (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).VerQty = Tess.VerQty;

  neut_tess_geoVerCoo (Tess, pGeo);
  neut_tess_geoVerEdge (Tess, pGeo);
  neut_tess_geoVerState (pGeo);

  return;
}

/* neut_tess_geotializing the properties of the edges */
void
neut_tess_geo_edge (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).EdgeQty = Tess.EdgeQty;

  neut_tess_geoEdgeVerNb (Tess, pGeo);
  neut_tess_geoEdgeFace (Tess, pGeo);
  neut_geo_init_edgelength (pGeo);
  neut_tess_geoEdgeState (pGeo);
  neut_tess_geoEdgeDel (pGeo);

  return;
}

/* neut_tess_geotializing the properties of the faces */
void
neut_tess_geo_face (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).FaceQty = Tess.FaceQty;

  neut_tess_geoFacePoly (Tess, pGeo);
  neut_tess_geoFaceVer (Tess, pGeo);
  neut_tess_geoFaceEdge (Tess, pGeo);
  neut_tess_geoFaceFF  (pGeo); 
  neut_tess_geoFaceState (pGeo); 
  neut_tess_geoFacePt (pGeo); 
  neut_tess_geoFaceEq (Tess, pGeo);

  return;
}

/* neut_tess_geotializing the properties of the polyhedra */
void
neut_tess_geo_poly (struct TESS Tess, struct GEO *pGeo)
{
  (*pGeo).PolyQty = Tess.PolyQty;
  
  (*pGeo).CenterCoo = ut_alloc_2d (Tess.PolyQty + 1, 3);
  (*pGeo).PolyTrue  = ut_alloc_1d_int (Tess.PolyQty + 1);
  (*pGeo).PolyBody  = ut_alloc_1d_int (Tess.PolyQty + 1);
  
  ut_array_2d_memcpy ((*pGeo).CenterCoo, Tess.PolyQty + 1, 3, Tess.CenterCoo);

  ut_array_1d_int_memcpy ((*pGeo).PolyTrue, Tess.PolyQty + 1, Tess.PolyTrue);
  ut_array_1d_int_memcpy ((*pGeo).PolyBody, Tess.PolyQty + 1, Tess.PolyBody);

  neut_tess_geoPolyFace (Tess, pGeo);

  return;
}
