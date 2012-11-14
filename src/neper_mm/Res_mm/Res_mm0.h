/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void Res_mm (struct IN, struct GEO, struct NODES,
                    struct MESH*, struct MESH*, struct MESH*, struct MESH*,
		    int**, struct GERMSET);

extern void Res_mm_tr (struct IN In, struct GERMSET GermSet, int SiteQty, int **SiteType, double **SiteCoo);
