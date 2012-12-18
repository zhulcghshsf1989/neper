/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MESH_FPRINTF_H
#define UT_MESH_FPRINTF_H

#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nodes_struct.h"
#include"../neut_structs/neut_part_struct.h"

/// \brief Print out a mesh at the Gmsh format.
///
///
///
extern void neut_mesh_fprintf_gmsh (FILE* file, char* dim, struct NODES
    Nodes, struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, struct PART Part);

/// \brief Print out a mesh at the Abaqus inp format.
///
///
///
extern void neut_mesh_fprintf_inp (FILE* file, char* dim, struct NODES Nodes,
    struct MESH Mesh0D, struct MESH Mesh1D, struct MESH Mesh2D,
    struct MESH Mesh3D, struct NSET NSet0D, struct NSET NSet1D,
    struct NSET NSet2D, char* nset);

/// \brief Print out a mesh at the geof format.
///
///
///
extern void neut_mesh_fprintf_geof (FILE* file, struct NODES Nodes,
    struct MESH Mesh2D, struct MESH Mesh3D, struct NSET NSet0D, struct
    NSET NSet1D, struct NSET NSet2D, char* nsets, int liset, int part,
    struct PART Part);

/// \brief Print out a mesh at the Fepx format.
///
///
///
extern void neut_mesh_fprintf_fepx_name (char* format, char* body, struct TESS
    Tess, struct NODES Nodes, struct MESH Mesh2D, struct MESH Mesh3D,
    struct NSET NSet0D, struct NSET NSet1D, struct NSET NSet2D, char
    *nset, char* faset);

/// \brief Print out a mesh at the Scotch format.
///
///
///
extern void neut_mesh_fprintf_scotch (char* body, struct NODES Nodes,
    struct MESH Mesh);

/// \brief Print out a mesh at the Stellar format.
///
///
///
extern void neut_mesh_fprintf_stellar (char* body, struct NODES Nodes,
    int* BoundNodes, struct MESH Mesh3D);

/// \brief Print out a Stellar configuration file.
///
///
///
extern void neut_mesh_fprintf_stellar_config (FILE* file);

#endif /* NEUT_MESH_FPRINTF_H */
