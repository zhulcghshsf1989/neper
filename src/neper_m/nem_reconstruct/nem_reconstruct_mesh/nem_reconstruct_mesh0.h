/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_reconstruct_mesh (char*, struct NODES*, struct MESH*, struct MESH*, struct MESH*, struct MESH*, struct TESS*);

extern void nem_reconstruct_mesh_tess_updating (struct TESS* pTess, struct NODES
    RNodes, struct MESH RMesh0D, struct MESH RMesh1D, struct MESH
    RMesh2D, struct MESH RMesh3D);
