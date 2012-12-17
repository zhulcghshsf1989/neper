/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MESH_QUAD_H
#define UT_MESH_QUAD_H

#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nset_struct.h"

/// \brief Create a 3D mesh into quad (hex) elts.
///
///
///
extern void neut_mesh_quad (int *size, int order, struct NODES *pNodes, struct MESH
    *pMesh, struct NSET* pNSet2D);

#endif /* UT_MESH_QUAD_H */
