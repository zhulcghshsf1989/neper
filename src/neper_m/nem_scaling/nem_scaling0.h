/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_scaling_pre (char* elttype, struct TESS* pTess, struct VOX* pVox,
    struct NODES* pRNodes, struct MESH RMesh0D, struct MESH RMesh1D,
    struct MESH RMesh2D, struct MESH RMesh3D, struct MESHPARA* pMeshPara);

extern void nem_scaling_post (struct MESHPARA MeshPara, struct TESS* pTess,
			      struct VOX* pVox, struct NODES* pNodes);
