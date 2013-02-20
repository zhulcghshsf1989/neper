/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TESS_OP_H
#define NEUT_TESS_OP_H

/// \brief Set a TESS structure to zero
///
///
///
extern void neut_tess_set_zero (struct TESS * pTess);

/// \brief Free a TESS structure
///
///
///
extern void neut_tess_free (struct TESS* pTess);

/// \brief Scale a TESS in the 3 directions of space
///
///
extern void neut_tess_scale (struct TESS* pTess, double x, double y, double z);

/// \brief Switch two polyhedra of a TESS
///
///
extern void neut_tess_poly_switch (struct TESS*, int poly1, int poly2);

/// \brief Initialize the true level of the polyhedra of a TESS
///
///
///
extern void neut_tess_init_polytrue (struct TESS* pTess);

/// \brief Initialize the body level of the polyhedra of a TESS
///
///
///
extern void neut_tess_init_polybody (struct TESS* pTess);


/// \brief Initialize the lengths of the edges of a TESS
///
///
///
extern void   neut_tess_init_edgelength (struct TESS* pTess);

/// \brief Initialize the length of an edge of a TESS
///
///
///
extern void neut_tess_init_edgelength_edge (struct TESS* pTess, int edge);

/// \brief Initialize PolyQty, PolyFaceQty and PolyFaceNb from FacePoly
/// 
/// 
/// 
extern void   neut_tess_init_polystuff_fromfacepoly (struct TESS* pTess);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
/// 
/// 
/// 
extern void   neut_tess_init_edgestuff_fromver (struct TESS* pTess);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
/// 
/// 
/// 
extern void   neut_tess_init_facestuff_fromedge (struct TESS* pTess);

/// \brief Initialize FaceEdgeOri and FaceVerNb; permuting FaceEdgeNb; initialize FacePt, FacePtCoo and FaceFF
///
///
///
extern void   neut_tess_init_facestuff_fromver (struct TESS* pTess);

/// \brief Initialize BoundingBox
///
///
///
extern void   neut_tess_init_bbox (struct TESS* pTess);

/// \brief Initialize FaceDom
///
/// Requirements: uses FacePoly
///
/// Returned value: amount of domain faces
///
extern int    neut_tess_init_facedom (struct TESS* pTess);

/// \brief Initialize FaceDom of a face using VerDom
///
/// Requirements: uses VerDom (and the TESS non-domain variables)
///
extern void   neut_tess_init_facedom_face_v (struct TESS* pTess, int face);

/// \brief Initialize EdgeDom
///
/// Requirements: uses the Tess (incl. Domain) definition and FaceDom
///
///
extern void   neut_tess_init_edgedom (struct TESS* pTess);

/// \brief Initialize VerDom
///
/// Requirements: uses the Tess (incl. Domain) definition and FaceDom
///
///
extern void   neut_tess_init_verdom (struct TESS* pTess);

/// \brief Initialize the face flatness fault of a face
/// 
/// 
///
extern double neut_tess_init_faceff_face (struct TESS* pTess, int face);

/// \brief Initialize DomTessFaceQty and DomTessFaceNb
/// 
/// Uses FaceDom
///
extern void neut_tess_init_domtessface (struct TESS* pTess);

/// \brief Initialize DomTessEdgeQty and DomTessEdgeNb
/// 
/// Requirements: uses DomEdgeFaceNb, DomTessFaceQty and DomTessFaceNb
///
extern void neut_tess_init_domtessedge (struct TESS* pTess);

/// \brief Initialize DomTessVerNb
/// 
/// Uses VerDom
///
extern void neut_tess_init_domtessver (struct TESS* pTess);

/// \brief Initialize the domain of a TESS
/// 
/// Input requirements: pointer to a TESS structure whose all non-domain
/// variables have been initialized.
/// 
/// Output: pointer to a TESS structure whose all *Dom* parameters are initialized.
///
extern void neut_tess_init_domain (struct TESS* pTess);

/// \brief Initialize the domain of a TESS using a POLY
/// 
///
///
extern void neut_tess_init_domain_poly (struct TESS* pTess, struct POLY Domain, char* type);

/// \brief Initialize the domain of a TESS using a POLY: simple copy, no initialization of the Tess to Domain variables (*Dom and DomTess*)
/// 
///
///
extern void neut_tess_init_domain_memcpy (struct TESS* pTess, struct POLY Domain, char* type);

/// \brief Initialize the face labels of the domain of a TESS.
///
///
///
extern void neut_tess_init_domain_facelabel (struct TESS* pTess);

/// \brief Sort the polys of a tessellation.
///
///
///
extern void neut_tess_sort (struct TESS* pTess, char* entity, char* expr);

extern void neut_tess_cat (struct TESS* pTessA, struct TESS TessB);

#endif /* NEUT_TESS_OP_H */
