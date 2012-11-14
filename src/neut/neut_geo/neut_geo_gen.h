/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_GEO0_H
#define NEUT_GEO0_H

#include"../neut_structs.h"

/// \brief Get the faces of a vertex
///
///
///
extern void neut_geo_ver_faces (struct GEO Geo, int ver, int** pface, int* pfaceqty);

/// \brief Get the polyhedra of a vertex
///
///
///
extern void neut_geo_ver_polys (struct GEO Geo, int ver, int** ppoly, int* ppolyqty);

/// \brief Get the domain faces of a vertex
///
///
///
extern void neut_geo_ver_domfaces (struct GEO Geo, int ver, int** pdface, int* pdfaceqty);

/// \brief Get the polyhedra of an edge
///
///
///
extern void neut_geo_edge_polys (struct GEO Geo, int edge, int** ppoly, int* ppolyqty);

/// \brief Get the vertices of a set of edges
///
///
///
extern void neut_geo_edges_vers (struct GEO Geo, int* edge, int edgeqty, int** pver, int* pverqty);

/// \brief Tell if an edge is a fake edge (not a triple line + on the circular part of a cylinder)
///
///
///
extern int neut_geo_edge_fake (struct GEO Geo, int edge);

/// \brief Get the edges of a set of faces
///
///
///
extern void neut_geo_faces_edges (struct GEO Geo, int* face, int faceqty, int** pedge, int* pedgeqty);

/// \brief Get the common edge of a set of faces of a GEO
///
///
///
extern int neut_geo_faces_commonedge (struct GEO Geo, int* face, int faceqty, int* pedge);

/// \brief Get the vertices of a polyhedron
///
///
///
extern void neut_geo_poly_vers (struct GEO Geo, int poly, int** pver, int* pverqty);

/// \brief Get the edges of a polyhedron
///
///
///
extern void neut_geo_poly_edges (struct GEO Geo, int poly, int** pedge, int* pedgeqty);

/// \brief Get the skin faces of a set of polyhedra
///
///
///
extern void neut_geo_polys_skinfaces (struct GEO Geo, int* poly, int polyqty, int** pface, int** pfacepoly, int* pfaceqty);

/// \brief Get the neighbouring polyhedra of a polyhedron
///
///
///
extern void neut_geo_poly_neighpoly (struct GEO Geo, int poly, int** pnpoly, int* pnpolyqty);

/// \brief Get the two faces of an edge of a polyhedron
///
///
///
extern void neut_geo_poly_edge_faces (struct GEO Geo, int poly, int edge, int** pface);

/// \brief Get the orientation of a face of a polyhedron
///
///
///
extern void neut_geo_poly_face_ori (struct GEO Geo, int poly, int face, int* pfaceori);

/// \brief Get the orientation of an edge of a face
///
///
///
extern void neut_geo_face_edge_ori (struct GEO Geo, int face, int edge, int* pedgeori);


/// \brief Get the common domain edge of a set of domain vertices
///
///
///
extern int neut_geo_domvers_commondomedge (struct GEO Geo, int* dver, int dverqty, int* pdedge);

/// \brief Get the domain faces of a domain vertex
///
///
///
extern void neut_geo_domver_domface (struct GEO Geo, int dver, int** pdface, int* pdfaceqty);

/// \brief Get the common domain face of a set of domain edges
///
///
///
extern int neut_geo_domedges_commondomface (struct GEO Geo, int* dedge, int dedgeqty, int* pdface);

/// \brief Get the domain vertices of a set of domain edges
///
///
///
extern int neut_geo_domedges_domvers (struct GEO Geo, int* dedge, int dedgeqty, int** pdver, int* pdverqty);

/// \brief Get the common domain vertex of a set of domain edges
///
///
///
extern int neut_geo_domedges_commondomver (struct GEO Geo, int* dedge, int dedgeqty, int* pdver);

/// \brief Get the vertices of a domain edge
///
///
///
extern void neut_geo_domedge_ver (struct GEO Geo, int dedge, int **pver, int* pverqty);

/// \brief Get the common domain edge of a set of domain faces
///
///
///
extern int neut_geo_domfaces_commondomedge (struct GEO Geo, int* dface, int dfaceqty, int* pdedge);

/// \brief Get the common domain ver of a set of domain faces
///
///
///
extern int neut_geo_domfaces_commondomver (struct GEO Geo, int* face, int faceqty, int* pver);

/// \brief Get the vertices of a domain face
///
///
///
extern void neut_geo_domface_ver (struct GEO Geo, int dface, int **pver, int* pverqty);

/// \brief Get the edges of a domain face
///
///
///
extern void neut_geo_domface_edges (struct GEO Geo, int dface, int **pedge, int* pedgeqty);

/// \brief Get the true level of a vertex
///
///
///
extern int neut_geo_ver_true (struct GEO Geo, int*, int);

/// \brief Get the maximum true level of the polyhedra of a vertex
///
///
///
extern int neut_geo_ver_polytruelevelmax (struct GEO Geo, int*, int);

/// \brief Get the true level of an edge
///
///
///
extern int neut_geo_edge_true (struct GEO Geo, int*, int);

/// \brief Get the maximum true level of the polyhedra of an edge
///
///
///
extern int neut_geo_edge_polytruelevelmax (struct GEO Geo, int*, int);

/// \brief Get the true level of a face
///
///
///
extern int neut_geo_face_true (struct GEO Geo, int*, int);

/// \brief Get the maximum true level of the polyhedra of a face
///
///
///
extern int neut_geo_face_polytruelevelmax (struct GEO Geo, int);

/// \brief Get the true level of a polyhedron
///
///
///
extern int neut_geo_poly_true (struct GEO Geo, int);

/// \brief Get the body level of a polyhedron
///
///
///
extern int neut_geo_poly_body (struct GEO Geo, int);

/// \brief Get the maximum body level of the polyhedra of a vertex
///
///
///
extern int neut_geo_ver_polybodylevelmax (struct GEO Geo, int*, int);

/// \brief Get the maximum body level of the polyhedra of an edge
///
///
///
extern int neut_geo_edge_polybodylevelmax (struct GEO Geo, int*, int);

/// \brief Get the maximum body level of the polyhedra of a face
///
///
///
extern int neut_geo_face_polybodylevelmax (struct GEO Geo, int);
/// \brief Get the centre of an edge
///
///
///
extern int neut_geo_edge_centre (struct GEO Geo, int, double*);

/// \brief Get the area of a face
///
///
///
extern int neut_geo_face_area (struct GEO Geo, int, double*);

/// \brief Get the centre of a face
///
///
///
extern void neut_geo_face_centre (struct GEO Geo, int, double*);

/// \brief Get the volume of a polyhedron
///
///
///
extern int neut_geo_poly_volume (struct GEO Geo, int, double*);

/// \brief Get the centroid of a polyhedron
///
///
///
extern int neut_geo_poly_centroid (struct GEO Geo, int, double*);

/// \brief Get the volume of a GEO
///
///
///
extern int neut_geo_volume (struct GEO Geo, double*);

/// \brief Get the centre of a GEO
///
///
///
extern void neut_geo_centre (struct GEO Geo, double*);

/// \brief Get the bounding box of a GEO
///
///
///
extern void neut_geo_boundingbox (struct GEO Geo, double**);

/// \brief Determine whether a point is within a polyhedron 
///
///
///
extern int neut_geo_point_inpoly (struct GEO Geo, double*, int);

/// \brief Determine if a polyhedron is standard
///
///
///
extern int neut_geo_poly_std (struct GEO Geo, int);

/// \brief Determine if a polyhedron has been regularized
///
///
///
extern int neut_geo_poly_reg (struct GEO Geo, int);

extern int neut_geo_expr_polytab (struct GEO Geo, char*, int*);
extern int neut_geo_expr_facetab (struct GEO Geo, char*, int*);
extern int neut_geo_face_interpolmesh (struct GEO Geo, int, struct NODES*, struct MESH*);
extern int neut_geo_edge_selratio (struct GEO Geo, struct GEOPARA, int, double*);

#endif /* NEUT_GEO0_H */
