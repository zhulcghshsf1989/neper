/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MESH_GEN_H
#define UT_MESH_GEN_H

#include"../neut_structs/neut_geo_struct.h"
#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nodes_struct.h"

/// \brief Create a mesh containing an element of a mesh.
///
///
///
extern int neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh, int elt,
                    struct NODES* pENodes, struct MESH* pEMesh);

/// \brief Create a mesh containing an elset of a mesh.
///
///
///
extern void neut_mesh_elset_mesh (struct NODES Nodes, struct MESH Mesh,
    int elset, struct NODES* pENodes, struct MESH* pEMesh);

/// \brief Determine the elt of a mesh to which a point belongs.
///
///
///
extern int neut_mesh_point_elt (struct MESH Mesh, struct NODES Nodes,
    double* coo, int* pelt);

/// \brief Determine the elt of an elset of a mesh to which a point belongs.
///
///
///
extern int neut_mesh_elset_point_elt (struct MESH Mesh, struct NODES
    Nodes, int elset, double* coo, int* pelt);

/// \brief Test if a point belongs to an elt of a mesh.
///
///
///
extern void neut_mesh_point_elt_in (struct MESH Mesh, struct NODES
    Nodes, double* coo, int elt, int* pinelt);

/// \brief Determine the clsest elt of an elset of a mesh to which a point is.
///
///
///
extern int neut_mesh_elset_point_closestelt (struct MESH Mesh, struct
    NODES Nodes, int elset, double* coo, int* pelt);

/// \brief Project a point along a given direction onto a mesh.
///
///
///
extern int neut_mesh_point_proj_alongonto (double* Coo, double* n, struct
    NODES N, struct MESH M, int elset);

/// \brief Get the mesh of the boundary of a GEO face. 
///
/// 
///
extern void neut_mesh_face_boundmesh (struct MESH Mesh1D, struct GEO Geo, int face,
    struct MESH* pBMesh);

/// \brief Get the nodes of the boundary of a Geo face.
///
/// 
///
extern void neut_mesh_face_boundnodes (struct MESH Mesh1D, struct GEO
    Geo, int face, int** pnodes, int* pnodeqty);

/// \brief Get the mesh of the boundary of a GEO poly. 
///
/// 
///
extern void neut_mesh_poly_boundmesh (struct GEO Geo, int poly, struct
    MESH Mesh2D, struct MESH* pBMesh);

/// \brief Create a 2D mesh from a slice of a 3D mesh.
///
///
///
extern void neut_mesh3d_slice (struct NODES Nodes, struct MESH Mesh,
    double* eq, struct NODES* pSNodes, struct MESH* pSMesh, int**
    pelt_newold, int*** pnode_newold, double** pnode_fact);

#endif /* UT_MESH_GEN_H */
