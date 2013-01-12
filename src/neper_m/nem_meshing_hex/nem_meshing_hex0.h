/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void
nem_meshing_tess_hex (struct IN In, struct MESHPARA MeshPara, struct TESS Tess, 
		      struct NODES *pNodes, struct MESH* pMesh0D,
		      struct MESH* pMesh1D, struct MESH* pMesh2D, 
		      struct MESH *pMesh3D, struct NSET* pNSet2D);

// extern void nemm_in_msize (struct IN, double**, int*);
extern void
nem_meshing_vox_hex (struct IN In, struct MESHPARA MeshPara, struct VOX Vox, 
		     struct NODES* pNodes,
		     struct MESH* pMesh0D, struct MESH* pMesh1D,
		     struct MESH* pMesh2D, struct MESH* pMesh3D,
		     struct NSET* pNSet2D);
