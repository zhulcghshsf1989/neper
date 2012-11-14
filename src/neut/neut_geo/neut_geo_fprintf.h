/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  GEOEXPORT0_H
#define  GEOEXPORT0_H

extern void neut_geo_fprintf (FILE*, struct GEO);
extern void neut_geo_fprintf_gmsh (FILE*, struct GEO);
extern void neut_geo_fprintf_ply  (FILE*, struct GEO);
extern void neut_geo_fprintf_dec  (FILE*, struct GEO);

#endif /* GEOEXPORT0_H */
