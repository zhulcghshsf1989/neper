/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_NODES_H
#define UT_NODES_H

#include"neut_structs.h"

/// \brief Free a NODES structure.
///
///
///
extern void neut_nodes_free (struct NODES* pNodes);

/// \brief Set a NODES structure to zero.
///
///
///
extern void neut_nodes_set_zero (struct NODES*);

/// \brief Copy a NODES structure to another NODES structure.
///
///
///
extern void neut_nodes_memcpy (struct NODES Nodes, struct NODES* pNodes2);

/// \brief Scale the coordinates of nodes in the 3 directions of space.
///
///
///
extern void neut_nodes_scale (struct NODES *pNodes, double scalex,
    double scaley, double scalez);

/// \brief Switch two nodes.
///
///
///
extern void neut_nodes_switch_pair (struct NODES *pNodes, int n1, int n2);

/// \brief Switch nodes.
///
///
///
extern void neut_nodes_switch (struct NODES *pNodes, int* node_nbs);

/// \brief Initialize the bounding box of nodes.
///
///
///
extern void neut_nodes_init_bbox (struct NODES* pNodes);

/// \brief Add a node to a NODES structure.
///
///
///
extern void neut_nodes_addnode (struct NODES* pNodes, double* NodeCoo,
    double NodeCl);

/// \brief Project nodes along a given direction onto a Mesh.
///
///
///
extern void neut_nodes_proj_alongontomesh (struct NODES *pN, double* n,
    struct NODES N, struct MESH M, int elset);

/// \brief Remove orphan nodes. 
///
///
///
extern int neut_nodes_rmorphans (struct NODES* pNodes, struct MESH*
    pMesh, struct NSET* pNSet2D);

/// \brief Compute the bounding box of nodes.
///
///
///
extern void neut_nodes_bbox (struct NODES Nodes, double *bbox);

/// \brief Get the closest node of a point.
///
///
///
extern int neut_nodes_point_closestnode (struct NODES Nodes, double* coo, int* pnode);

/// \brief Get the distance between two nodes.
///
///
///
extern double neut_nodes_dist_pair (struct NODES Nodes, int n1, int n2);

#endif
