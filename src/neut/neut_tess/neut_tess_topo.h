/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TESS_TOPO_H
#define NEUT_TESS_TOPO_H

#include"../neut_structs.h"

/// \brief Get the faces of a vertex
///
///
///
extern void neut_tess_ver_faces (struct TESS Tess, int ver, int** pface, int* pfaceqty);

/// \brief Get the faces of a vertex, among the faces of a poly
///
///
///
extern void neut_tess_ver_poly_faces (struct TESS Tess, int ver, int
    poly, int** pface, int* pfaceqty);

/// \brief Get the polyhedra of a vertex
///
///
///
extern void neut_tess_ver_polys (struct TESS Tess, int ver, int** ppoly, int* ppolyqty);

/// \brief Get the domain faces of a vertex
///
///
///
extern void neut_tess_ver_domfaces (struct TESS Tess, int ver, int** pdface, int* pdfaceqty);

/// \brief Get the polyhedra of an edge
///
///
///
extern void neut_tess_edge_polys (struct TESS Tess, int edge, int** ppoly, int* ppolyqty);

/// \brief Get the vertices of a set of edges
///
///
///
extern void neut_tess_edges_vers (struct TESS Tess, int* edge, int edgeqty, int** pver, int* pverqty);

/// \brief Tell if an edge is a fake edge (not a triple line + on the circular part of a cylinder)
///
///
///
extern int neut_tess_edge_fake (struct TESS Tess, int edge);

/// \brief Get the edges of a set of faces
///
///
///
extern void neut_tess_faces_edges (struct TESS Tess, int* face, int faceqty, int** pedge, int* pedgeqty);

/// \brief Get the com edge of a set of faces of a TESS
///
///
///
extern int neut_tess_faces_comedge (struct TESS Tess, int* face, int faceqty, int* pedge);

/// \brief Get the vertices of a polyhedron
///
///
///
extern void neut_tess_poly_vers (struct TESS Tess, int poly, int** pver, int* pverqty);

/// \brief Get the edges of a polyhedron
///
///
///
extern void neut_tess_poly_edges (struct TESS Tess, int poly, int** pedge, int* pedgeqty);

/// \brief Get the skin faces of a set of polyhedra
///
///
///
extern void neut_tess_polys_skinfaces (struct TESS Tess, int* poly, int polyqty, int** pface, int** pfacepoly, int* pfaceqty);

/// \brief Get the neighbouring polyhedra of a polyhedron
///
///
///
extern void neut_tess_poly_neighpoly (struct TESS Tess, int poly, int** pnpoly, int* pnpolyqty);

/// \brief Get the two faces of an edge of a polyhedron
///
///
///
extern void neut_tess_poly_edge_faces (struct TESS Tess, int poly, int edge, int** pface);

/// \brief Get the orientation of a face of a polyhedron
///
///
///
extern void neut_tess_poly_face_ori (struct TESS Tess, int poly, int face, int* pfaceori);

/// \brief Get the orientation of an edge of a face
///
///
///
extern void neut_tess_face_edge_ori (struct TESS Tess, int face, int edge, int* pedgeori);


/// \brief Get the com domain edge of a set of domain vertices
///
///
///
extern int neut_tess_domvers_comdomedge (struct TESS Tess, int* dver, int dverqty, int* pdedge);

/// \brief Get the domain faces of a domain vertex
///
///
///
extern void neut_tess_domver_domface (struct TESS Tess, int dver, int** pdface, int* pdfaceqty);

/// \brief Get the com domain face of a set of domain edges
///
///
///
extern int neut_tess_domedges_comdomface (struct TESS Tess, int* dedge, int dedgeqty, int* pdface);

/// \brief Get the domain vertices of a set of domain edges
///
///
///
extern int neut_tess_domedges_domvers (struct TESS Tess, int* dedge, int dedgeqty, int** pdver, int* pdverqty);

/// \brief Get the com domain vertex of a set of domain edges
///
///
///
extern int neut_tess_domedges_comdomver (struct TESS Tess, int* dedge, int dedgeqty, int* pdver);

/// \brief Get the vertices of a domain edge
///
///
///
extern void neut_tess_domedge_ver (struct TESS Tess, int dedge, int **pver, int* pverqty);

/// \brief Get the com domain edge of a set of domain faces
///
///
///
extern int neut_tess_domfaces_comdomedge (struct TESS Tess, int* dface, int dfaceqty, int* pdedge);

/// \brief Get the com domain ver of a set of domain faces
///
///
///
extern int neut_tess_domfaces_comdomver (struct TESS Tess, int* face, int faceqty, int* pver);

/// \brief Get the vertices of a domain face
///
///
///
extern void neut_tess_domface_ver (struct TESS Tess, int dface, int **pver, int* pverqty);

/// \brief Get the edges of a domain face
///
///
///
extern void neut_tess_domface_edges (struct TESS Tess, int dface, int **pedge, int* pedgeqty);

/// \brief Get the true level of a vertex
///
///
///
extern int neut_tess_ver_true (struct TESS Tess, int*, int);

/// \brief Get the maximum true level of the polyhedra of a vertex
///
///
///
extern int neut_tess_ver_polytruelevelmax (struct TESS Tess, int*, int);

/// \brief Get the true level of an edge
///
///
///
extern int neut_tess_edge_true (struct TESS Tess, int*, int);

/// \brief Get the maximum true level of the polyhedra of an edge
///
///
///
extern int neut_tess_edge_polytruelevelmax (struct TESS Tess, int*, int);

/// \brief Get the true level of a face
///
///
///
extern int neut_tess_face_true (struct TESS Tess, int*, int);

/// \brief Get the maximum true level of the polyhedra of a face
///
///
///
extern int neut_tess_face_polytruelevelmax (struct TESS Tess, int);

/// \brief Get the true level of a polyhedron
///
///
///
extern int neut_tess_poly_true (struct TESS Tess, int);

/// \brief Get the body level of a polyhedron
///
///
///
extern int neut_tess_poly_body (struct TESS Tess, int);

/// \brief Get the maximum body level of the polyhedra of a vertex
///
///
///
extern int neut_tess_ver_polybodylevelmax (struct TESS Tess, int*, int);

/// \brief Get the maximum body level of the polyhedra of an edge
///
///
///
extern int neut_tess_edge_polybodylevelmax (struct TESS Tess, int*, int);

/// \brief Get the maximum body level of the polyhedra of a face
///
///
///
extern int neut_tess_face_polybodylevelmax (struct TESS Tess, int);


/// \brief Determine if a polyhedron is standard
///
///
///
extern int neut_tess_poly_std (struct TESS Tess, int);

/// \brief Determine if a polyhedron has been regularized
///
///
///
extern int neut_tess_poly_reg (struct TESS Tess, int);

/// \brief Get a mesh from a poly of a TESS
///
///
///
extern int
neut_tess_poly_mesh (struct TESS Tess, int nb,
		    struct NODES* pNodes, struct MESH* pMesh);

extern int neut_tess_expr_polytab (struct TESS Tess, char*, int*);
extern int neut_tess_expr_facetab (struct TESS Tess, char*, int*);
extern int neut_tess_face_interpolmesh (struct TESS Tess, int, struct NODES*, struct MESH*);

#endif /* NEUT_TESS_TOPO_H */
