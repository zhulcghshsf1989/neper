/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_meshing_3D (struct IN, struct MESHPARA, struct TESS,
    struct NODES *, struct MESH, struct MESH *);
extern int nem_meshing_3D_poly_cl (struct MESHPARA, struct TESS, int, double*);

