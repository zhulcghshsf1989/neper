/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_reconmesh (char*, struct NODES*, struct MESH*, struct MESH*, struct MESH*, struct MESH*, struct TESS*);
extern void nem_reconmesh_finalizetess (struct TESS*, struct NODES,
    struct MESH, struct MESH, struct MESH, struct MESH);
