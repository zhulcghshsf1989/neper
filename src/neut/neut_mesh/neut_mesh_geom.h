/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_MESH_GEOM_H
#define NEUT_MESH_GEOM_H

#include"../neut_structs/neut_mesh_struct.h"
#include"../neut_structs/neut_nodes_struct.h"

/// \brief Get the centre of an elt of a mesh.
///
///
///
extern void neut_mesh_elt_centre (struct MESH Mesh, struct NODES Nodes, int elt, double *coo);

/// \brief Get the length of an elt of a 1D mesh.
///
///
///
extern int neut_mesh_elt_length (struct NODES Nodes, struct MESH Mesh, int elt, double *plength);

/// \brief Get the average, min and max lengths of an elt of a 2D or 3D mesh.
///
///
///
extern void neut_mesh_elt_lengths (struct MESH Mesh, struct NODES Nodes,
    int elt, double* pavlength, double* pminlength, double* pmaxlength);

/// \brief Get the area of an elt of a 2D mesh.
///
///
///
extern int neut_mesh_elt_area (struct NODES Nodes, struct MESH Mesh, int elt, double* parea);

/// \brief Get the volume of an elt of a 3D mesh.
///
///
///
extern int neut_mesh_elt_volume (struct NODES Nodes, struct MESH Mesh, int elt, double* pvol);

/// \brief Get the normal of an elt of a 2D mesh.
///
///
///
extern void neut_mesh_elt_normal (struct MESH Mesh, struct NODES Nodes, int elt, double *n);

/// \brief Get the equation of an elt of a 2D mesh.
///
///
///
extern void neut_mesh_elt_eq (struct MESH Mesh, struct NODES Nodes, int elt, double *eq);

/// \brief Get the equation of an elset of a 2D mesh.
///
/// The equation is the average of the equations of the elts, weighted
/// by their respective areas.
///
extern void neut_mesh_elset_eq (struct MESH Mesh, struct NODES Nodes, int elset, double *eq);

/// \brief Get the radius ratio of an elt of a 3D mesh.
///
///
///
extern int neut_mesh_elt_rr (struct NODES Nodes, struct MESH Mesh, int elt, double* prr);

/// \brief Get the centre of an elset of a 3D mesh.
///
///
///
extern int neut_mesh_elset_centre (struct NODES Nodes, struct MESH Mesh,
    int elset, double* centre);

/// \brief Get the volume of an elset of a 3D mesh.
///
///
///
extern int neut_mesh_elset_volume (struct NODES Nodes, struct MESH Mesh, int elset, double* pvol);

/// \brief Get the min, average and max radius ratios of the elts of an
/// elset of a mesh.
///
/// This function works for a 3D mesh.
///
extern int neut_mesh_elset_rr (struct NODES Nodes, struct MESH Mesh, int
    elset, double* prrmean, double* prrmin, double* prrmax);

/// \brief Get the Osize value of an elset of a mesh.
///
/// See (CMAME, 2011) for the definition of Osize.
///
extern int neut_mesh_elset_Osize (struct NODES Nodes, struct MESH Mesh, int elset,
			    double cl, double* pOsize);

/// \brief Get the centre of a 3D mesh.
///
///
///
extern void neut_mesh_centre (struct NODES Nodes, struct MESH Mesh, double* centre);

/// \brief Get the volume of a 3D mesh.
///
///
///
extern int neut_mesh_volume (struct NODES Nodes, struct MESH Mesh, double* pvol);

/// \brief Get the average length of the elts of a mesh.
///
/// 
///
extern int neut_mesh_eltlength (struct NODES Nodes, struct MESH Mesh,
    double* pavlength);


/// \brief Get the min, average and max radius ratios of the elts of a mesh.
///
/// This function works for 2D and 3D meshes.
///
extern int neut_mesh_rr (struct NODES Nodes, struct MESH Mesh,
    double* prrav, double* prrmin, double* prrmax);

/// \brief Get the Odis value of a mesh.
///
/// See (CMAME, 2011) for the definition of Odis.
///
extern int neut_mesh_Odis (struct NODES Nodes, struct MESH Mesh, char* Odisexpr, double* pOdis);

/// \brief Get the Osize value of a mesh.
///
/// See (CMAME, 2011) for the definition of Osize.
///
extern int neut_mesh_Osize (struct NODES Nodes, struct MESH Mesh, double cl, double* pOsize);

#endif /* NEUT_MESH_GEOM_H */
