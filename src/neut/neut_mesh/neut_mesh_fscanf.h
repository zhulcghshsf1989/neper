/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MESH_FSCANF_H
#define UT_MESH_FSCANF_H

#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nodes_struct.h"

/// \brief Read in a mesh at the Gmsh format.
///
///
///
extern void neut_mesh_fscanf_msh (FILE * file, struct NODES *pNodes,
    struct MESH *pMesh0D, struct MESH *pMesh1D, struct MESH *pMesh2D,
    struct MESH *pMesh3D);

/// \brief Read in a mesh at the geof format.
///
///
///
extern void neut_mesh_fscanf_geof (FILE * file, struct NODES *pNodes, struct MESH *pMesh);

/// \brief Read in a mesh at the Fepx format.
///
///
///
extern void neut_mesh_fscanf_fepx (FILE * parms, FILE * mesh, FILE * elsets,
		      struct NODES *pNodes, struct MESH *pMesh);

/// \brief Read in a mesh at the Stellar format.
///
///
///
extern void neut_mesh_fscanf_stellar (FILE * nodes, FILE * elts, struct
    NODES *pNodes, struct MESH *pMesh);

#endif /* NEUT_MESH_FSCANF_H */
