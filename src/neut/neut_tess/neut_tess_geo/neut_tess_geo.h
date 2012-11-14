/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"neut_tess_geo0.h"

extern void neut_tess_geo_general (struct TESS, struct GEO*);
extern void neut_tess_geo_ver (struct TESS, struct GEO *);
extern void neut_tess_geoVerQty (struct TESS, struct GEO *);
extern void neut_tess_geoVerCoo (struct TESS, struct GEO *);
extern void neut_tess_geoVerEdge (struct TESS, struct GEO *);
extern void neut_tess_geoVerState (struct GEO *);
extern void neut_tess_geo_edge (struct TESS, struct GEO *);
extern void neut_tess_geoEdgeVerNb (struct TESS, struct GEO *);
extern void neut_tess_geoEdgeFace (struct TESS, struct GEO *);
extern void neut_tess_geoEdgeState (struct GEO *);
extern void neut_tess_geoEdgeDel (struct GEO *);
extern void neut_tess_geo_face (struct TESS, struct GEO *);
extern void neut_tess_geoFaceQty (struct TESS, struct GEO *);
extern void neut_tess_geoFacePoly (struct TESS, struct GEO *);
extern void neut_tess_geoFaceVer (struct TESS, struct GEO *);
extern void neut_tess_geoFaceEdge (struct TESS, struct GEO *);
extern void neut_tess_geoFaceFF (struct GEO *);
extern void neut_tess_geoFaceState (struct GEO *);
extern void neut_tess_geoFacePt (struct GEO *);
extern void neut_tess_geoFaceEq (struct TESS, struct GEO *);
extern void neut_tess_geo_poly (struct TESS, struct GEO *);
extern void neut_tess_geoPolyQty (struct TESS, struct GEO *);
extern void neut_tess_geoPolyFace (struct TESS, struct GEO *);
