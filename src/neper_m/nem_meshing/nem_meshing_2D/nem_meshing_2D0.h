/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_2D (struct IN In, struct TESSPARA TessPara, struct TESS Tess,
           double** face_proj, int* face_op, struct NODES RNodes,
	   struct MESH RMesh2D, struct MESH Mesh1D,
	   struct NODES *pNodes, struct MESH *pMesh2D);
