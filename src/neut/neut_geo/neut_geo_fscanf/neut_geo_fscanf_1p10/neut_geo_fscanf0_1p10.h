/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  GEOIMPORT_1P10_0_H
#define  GEOIMPORT_1P10_0_H

extern void neut_geo_fscanf_1p10 (FILE*, struct GEO*);
extern void neut_geo_name_fscanf_1p10 (char*, struct GEO*);
extern void neut_geo_fscanf_verbosity_1p10 (FILE* file, struct GEO* pGeo, int verbosity);

#endif /* GEOIMPORT_1P10_0_H */
