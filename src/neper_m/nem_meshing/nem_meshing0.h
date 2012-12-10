/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern int
nem_meshing (struct IN In, struct GEOPARA GeoPara, struct GEO Geo, 
             struct NODES RNodes, struct MESH RMesh1D, struct MESH RMesh2D,
             struct NODES* pNodes, struct MESH* pMesh0D,
	     struct MESH* pMesh1D, struct MESH* pMesh2D,
	     struct MESH* pMesh3D);

/*
extern int
nem_vox_mesh (struct IN In, struct GEOPARA GeoPara, struct VOX Vox, 
	      struct NODES* pNodes, struct MESH* pMesh0D,
	      struct MESH* pMesh1D, struct MESH* pMesh2D,
	      struct MESH* pMesh3D);
*/
