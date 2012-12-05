/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void
nem_geo_mesh_hex (struct IN In, struct GEOPARA GeoPara, struct GEO Geo, 
		  struct NODES *pNodes, struct MESH* pMesh0D,
		  struct MESH* pMesh1D, struct MESH* pMesh2D, 
		  struct MESH *pMesh3D, struct NSET* pNSet2D);

// extern void nemm_in_msize (struct IN, double**, int*);
extern void
nem_vox_mesh_hex (struct IN In, struct GEOPARA GeoPara, struct VOX Vox, 
                  struct NODES* pNodes,
		  struct MESH* pMesh0D, struct MESH* pMesh1D,
		  struct MESH* pMesh2D, struct MESH* pMesh3D);
