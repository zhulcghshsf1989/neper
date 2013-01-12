/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_cleaning_singnodedup (struct MESH* pMesh, struct NODES*
    pNodes, int*** pFoDNodes);

extern void nem_cleaning_dupnodemerge (struct NODES* pNodes, struct
    MESH* pMesh0D, struct MESH* pMesh1D, struct MESH* pMesh2D, struct
    MESH* pMesh3D, double eps);
