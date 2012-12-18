/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_init_remesh (char*, char*, struct TESS*, struct NODES*,
    struct MESH*, struct MESH*, struct MESH*, struct MESH*);
extern void nem_init_scaling (char* elttype, struct TESS* pTess, struct VOX* pVox,
    struct NODES* pRNodes, struct MESH RMesh0D, struct MESH RMesh1D,
    struct MESH RMesh2D, struct MESH RMesh3D, struct TESSPARA* pTessPara);
extern void nem_post_scaling (struct TESSPARA TessPara, struct TESS* pTess,
			      struct VOX* pVox, struct NODES* pNodes);
extern void nem_init_mesh_tess_updating (struct TESS*, struct NODES
, struct MESH, struct MESH, struct MESH, struct MESH);
