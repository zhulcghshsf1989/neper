/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nevs_show_init (struct MTESS MTess,
	   struct TESS Tess, struct NODES Nodes, struct MESH Mesh0D,
	   struct MESH Mesh1D, struct MESH Mesh2D, struct MESH  Mesh3D,
	   int SQty, struct PRINT* pPrint);
extern void nevs_show_print_mprint (struct MTESS MTess, struct TESS* Tess,
                        struct PRINT Print, struct PRINT** pMPrint);

extern void nevs_show (char **, int *, struct TESS, struct NODES, struct MESH,
		       struct MESH, struct MESH, struct MESH, struct PRINT *);
