/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */ 
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_MESH_OP_H
#define NEUT_MESH_OP_H

#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nodes_struct.h"

/// \brief Allocate a MESH structure.
///
///
///
extern struct MESH
neut_mesh_alloc (int Dimension, char* EltType, int EltOrder, int EltQty,
		 int ElsetQty);

/// \brief Free a MESH structure.
///
///
///
extern void neut_mesh_free (struct MESH* pMesh);

/// \brief Set a MESH structure to zero.
///
///
///
extern void neut_mesh_set_zero (struct MESH* pMesh);

/// \brief Copy a mesh structure.
///
///
///
extern void neut_mesh_memcpy (struct MESH Old, struct MESH* pNew);

/// \brief Compare two mesh structures.
///
///
///
extern int neut_mesh_cmp (struct NODES N1, struct MESH M1, struct NODES N2, struct MESH M2);

/// \brief Scale a mesh.
///
///
///
extern void neut_mesh_scale (struct MESH* pMesh, double scalex, double
    scaley, double scalez);

/// \brief Shift a mesh.
///
///
///
extern void neut_mesh_shift (struct MESH* pMesh, double shiftx, double
    shifty, double shiftz);

#ifdef HAVE_LIBSCOTCH
#include <scotch.h>
/// \brief Convert a mesh into a mesh at the Scotch format.
///
///
///
extern void neut_mesh_scotchmesh (struct MESH, int, SCOTCH_Mesh*);
#endif

/// \brief Initialize NodeElts of a MESH structure.
///
///
///
extern void neut_mesh_init_nodeelts (struct MESH* pMesh, int NodeQty);

/// \brief Initialize EltElset of a MESH structure.
///
/// Input: Elsets
///
extern void neut_mesh_init_eltelset (struct MESH* pMesh, int* elset_nbs);

/// \brief Initialize Elsets of a MESH structure.
///
/// Input: EltElset
///
extern void neut_mesh_init_elsets (struct MESH* pMesh);

/// \brief Switch a mesh to 2nd order.
///
///
///
extern void neut_mesh_order1to2 (struct NODES* pNodes, struct MESH* pMesh1D,
                     struct MESH* pMesh2D, struct MESH* pMesh3D);

/// \brief Merge the elsets of a mesh.
///
///
///
extern void neut_mesh_mergeelsets (struct MESH* pEMesh);

/// \brief Order the elsets of a 2D mesh.
///
/// This function enable to get all elts pointing towards the same side
/// of an elset.
///
extern void neut_mesh_orderelsets (struct MESH* pMesh);

/// \brief Reverse the nodes of an elt of a mesh.
///
/// The functions works for a 2D mesh only. It is useful to get a
/// properly oriented normal.
///
extern void neut_mesh_elt_reversenodes (struct MESH *pMesh, int elt);

/// \brief Get a tri/tet mesh from a quad/hex mesh.
///
///
///
extern void neut_mesh_quad_tri (struct NODES Nodes, struct MESH Mesh, struct MESH* pTMesh);

/// \brief Add an elt to a mesh.
///
///
///
extern void neut_mesh_addelt (struct MESH *pMesh, int *EltNodes);

/// \brief Add an elset to a mesh.
///
///
///
extern void neut_mesh_addelset (struct MESH *pMesh, int *ElsetElts, int
    EltQty);


/// \brief Add an elt to an elset of a mesh.
///
///
///
extern void neut_mesh_elset_addelt (struct MESH *pMesh, int elset_nb,
    int elt);

/// \brief Remove an elt from a mesh.
///
///
///
extern int neut_mesh_rmelt (struct MESH* pMesh, int elt);

/// \brief Remove elts from a mesh.
///
///
///
extern int neut_mesh_rmelts (struct MESH* pMesh, struct NODES Nodes, int* rmelt, int rmeltqty);

/// \brief Remove an elset from a mesh.
///
///
///
extern int neut_mesh_rmelset (struct MESH* pMesh, struct NODES Nodes, int elset);

/// \brief Switch two elts of a mesh.
///
///
///
extern void neut_mesh_elts_switch_pair (struct MESH *pMesh, int n1, int n2);

/// \brief Switch elts of a mesh.
///
///
///
extern void neut_mesh_elts_switch (struct MESH *pMesh, int* elt_nbs);

/// \brief Switch nodes of a mesh.
///
///
///
extern void neut_mesh_nodes_switch (struct MESH* pMesh, int* node_nbs);

/// \brief Switch nodes, elts and elsets of a mesh.
///
///
///
extern void neut_mesh_switch (struct MESH *pMesh, int *nodes_old_new,
    int *elts_old_new, int* elsets_old_new);

/// \brief Renumber nodes, elts and elsets of a mesh so that they are numbered contiguously from 1.
///
///
///
extern void neut_mesh_renumber_continuous (struct MESH *pMesh, int*
    node_nbs, int* elt_nbs, int* elset_nbs);

#endif /* NEUT_MESH_OP_H */
