/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nefm_init_geo  (char*, struct GEO*, struct GEOPARA*);
extern void nefm_init_remesh (struct IN, struct GEO*, struct NODES*,
    struct MESH*, struct MESH*, struct MESH*, struct MESH*, struct
    GEOPARA*);
extern void nefm_init_remeshingmapping_geo_updating (struct GEO*, struct NODES
, struct MESH, struct MESH, struct MESH, struct MESH);
extern void nefm_init_remap  (struct IN, struct GEO*, struct NODES*,
    struct MESH*, struct MESH*, struct MESH*, struct MESH*);
