/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_init_remesh (char*, char*, struct GEO*, struct NODES*,
    struct MESH*, struct MESH*, struct MESH*, struct MESH*);
extern void nem_init_scaling (struct GEO* pGeo, struct VOX* pVox,
    struct NODES* pRNodes, struct MESH RMesh0D, struct MESH RMesh1D,
    struct MESH RMesh2D, struct MESH RMesh3D, struct GEOPARA* pGeoPara);
extern void nem_post_scaling (struct GEOPARA GeoPara, struct GEO* pGeo,
			      struct VOX* pVox, struct NODES* pNodes);
extern void nem_init_remeshingmapping_geo_updating (struct GEO*, struct NODES
, struct MESH, struct MESH, struct MESH, struct MESH);
