/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_geo_geo.h"

extern void neut_geo_geoGen (struct GEO, struct GEO *);
extern void neut_geo_geoVer (struct GEO, struct GEO *);
extern void neut_geo_geoVerQty (struct GEO, struct GEO *);
extern void neut_geo_geoVerCoo (struct GEO, struct GEO *);
extern void neut_geo_geoVerEdge (struct GEO, struct GEO *);
extern void neut_geo_geoVerBound (struct GEO, struct GEO *);
extern void neut_geo_geoVerState (struct GEO, struct GEO *);
extern void neut_geo_geoEdge (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeQty (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeVerNb (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeFace (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeLength (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeState (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeDel (struct GEO, struct GEO *);
extern void neut_geo_geoEdgeBound (struct GEO, struct GEO *);
extern void neut_geo_geoFace (struct GEO, struct GEO *);
extern void neut_geo_geoFaceQty (struct GEO, struct GEO *);
extern void neut_geo_geoFacePoly (struct GEO, struct GEO *);
extern void neut_geo_geoFaceVer (struct GEO, struct GEO *);
extern void neut_geo_geoFaceEdge (struct GEO, struct GEO *);
extern void neut_geo_geoFaceFF (struct GEO, struct GEO *);
extern void neut_geo_geoFaceEq (struct GEO, struct GEO *);
extern void neut_geo_geoFaceBound (struct GEO, struct GEO *);
extern void neut_geo_geoFaceState (struct GEO, struct GEO *);
extern void neut_geo_geoFacePt (struct GEO, struct GEO *);
extern void neut_geo_geoPoly (struct GEO, struct GEO *);
extern void neut_geo_geoPolyQty (struct GEO, struct GEO *);
extern void neut_geo_geoPolyFace (struct GEO, struct GEO *);
extern void neut_geo_geoPolyCenter (struct GEO, struct GEO *);
extern void neut_geo_geoPolyTrue (struct GEO, struct GEO *);
extern void neut_geo_geoPolyBody (struct GEO, struct GEO *);
extern void neut_geo_geoDomain (struct GEO, struct GEO *);
extern void neut_geo_geoDomainData (struct GEO, struct GEO *);
