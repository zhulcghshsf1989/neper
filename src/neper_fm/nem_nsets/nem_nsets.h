/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"ut.h"
#include"neut.h"
#include"../structIn_fm.h"

#include"nem_nsets0.h"

extern void nem_nsets_2d_geo (struct GEO, struct MESH, struct NSET*);
extern void nem_nsets_1d_geo (struct GEO, struct MESH, struct NSET, struct NSET*);
extern void nem_nsets_0d_geo (struct GEO, struct MESH, struct NSET, struct NSET*);
extern void nem_nsets_2dbody_geo (struct GEO, struct NSET, struct NSET*);
extern void nem_nsets_1dbody_geo (struct GEO, struct NSET, struct NSET*);

extern void nem_nsets_1d_geo_hex (struct GEO Geo, struct NSET NSet2D, struct NSET* pNSet1D);
extern void nem_nsets_0d_geo_hex (struct GEO Geo, struct NSET NSet2D, struct NSET  NSet1D,
                                  struct NSET* pNSet0D);
