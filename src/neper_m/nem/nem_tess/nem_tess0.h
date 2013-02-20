/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_tess_face_1delts (struct TESS, struct MESH, int, int**, int*);

extern void nem_tess_edgebodies_nodes (struct TESS Tess, struct MESH
		  Mesh0D, struct MESH Mesh1D, int** pnodes, int* pnodeqty);

extern void nem_tess_facebodies_nodes (struct TESS Tess, struct MESH
    Mesh1D, struct MESH Mesh2D, int** pnodes, int* pnodeqty);

extern void nem_tess_updatefrommesh_geom (struct TESS* pTess, struct
    NODES RNodes, struct MESH RMesh0D, struct MESH RMesh1D, struct MESH
    RMesh2D, struct MESH RMesh3D);

extern void nem_tess_updatefrommesh_polyfaceori (struct TESS* pTess,
    struct NODES RNodes, struct MESH RMesh2D, struct MESH RMesh3D);
