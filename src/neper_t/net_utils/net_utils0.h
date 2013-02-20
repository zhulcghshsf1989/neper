/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern int net_poly_clip (struct POLY* pPoly, double* eq);
extern void CreateTesl (struct GERMSET GermSet, double *Size, struct
    TESL *pTesl);
extern void net_tess_poly (struct TESS Tess, int poly, struct POLY* pPoly);
extern void net_poly_tesl (struct POLY Poly, struct TESL* pTess);
extern void net_poly_tess (struct POLY Poly, struct TESS* pTess);

extern void net_tess_poly_tess (struct TESS Tess, int poly, struct TESS* pPoly);

extern void net_poly_centroid (struct POLY Poly, double* coo);

#include "net_polymod_poly/net_polymod_poly0.h"
