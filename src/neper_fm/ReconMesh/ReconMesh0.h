/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void ReconMesh (char*, struct NODES*, struct MESH*, struct MESH*, struct MESH*, struct MESH*, struct GEO*);
extern void ReconMesh_finalizegeo (struct GEO*, struct NODES,
    struct MESH, struct MESH, struct MESH, struct MESH);
