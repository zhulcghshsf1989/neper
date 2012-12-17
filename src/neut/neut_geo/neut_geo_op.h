/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef  NEUT_GEO_OP_H
#define  NEUT_GEO_OP_H

/// \brief Set a GEO structure to zero
///
///
///
extern void neut_geo_set_zero (struct GEO * pGeo);

/// \brief Free a GEO structure
///
///
///
extern void neut_geo_free (struct GEO* pGeo);

/// \brief Scale a GEO in the 3 directions of space
///
///
extern void neut_geo_scale (struct GEO* pGeo, double x, double y, double z);

/// \brief Switch two polyhedra of a GEO
///
///
extern void neut_geo_poly_switch (struct GEO*, int poly1, int poly2);

/// \brief Initialize the true level of the polyhedra of a GEO
///
///
///
extern void neut_geo_init_polytrue (struct GEO* pGeo);

/// \brief Initialize the body level of the polyhedra of a GEO
///
///
///
extern void neut_geo_init_polybody (struct GEO* pGeo);


/// \brief Initialize the lengths of the edges of a GEO
///
///
///
extern void   neut_geo_init_edgelength (struct GEO* pGeo);

/// \brief Initialize the length of an edge of a GEO
///
///
///
extern void neut_geo_init_edgelength_edge (struct GEO* pGeo, int edge);

/// \brief Initialize PolyQty, PolyFaceQty and PolyFaceNb from FacePoly
/// 
/// 
/// 
extern void   neut_geo_init_polystuff_fromfacepoly (struct GEO* pGeo);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
/// 
/// 
/// 
extern void   neut_geo_init_edgestuff_fromver (struct GEO* pGeo);

/// \brief Initialize EdgeVerNb and EdgeLength from VerQty, VerEdgeQty and VerEdgeNb
/// 
/// 
/// 
extern void   neut_geo_init_facestuff_fromedge (struct GEO* pGeo);

/// \brief Initialize FaceEdgeOri and FaceVerNb; permuting FaceEdgeNb; initialize FacePt, FacePtCoo and FaceFF
///
///
///
extern void   neut_geo_init_facestuff_fromver (struct GEO* pGeo);

/// \brief Initialize BoundingBox
///
///
///
extern void   neut_geo_init_bbox (struct GEO* pGeo);

/// \brief Initialize FaceDom
///
/// Requirements: uses FacePoly
///
/// Returned value: amount of domain faces
///
extern int    neut_geo_init_facedom (struct GEO* pGeo);

/// \brief Initialize FaceDom of a face using VerDom
///
/// Requirements: uses VerDom (and the GEO non-domain variables)
///
extern void   neut_geo_init_facedom_face_v (struct GEO* pGeo, int face);

/// \brief Initialize EdgeDom
///
/// Requirements: uses the Geo (incl. Domain) definition and FaceDom
///
///
extern void   neut_geo_init_edgedom (struct GEO* pGeo);

/// \brief Initialize VerDom
///
/// Requirements: uses the Geo (incl. Domain) definition and FaceDom
///
///
extern void   neut_geo_init_verdom (struct GEO* pGeo);

/// \brief Initialize the face flatness fault of a face
/// 
/// 
///
extern double neut_geo_init_faceff_face (struct GEO* pGeo, int face);

/// \brief Initialize DomTessFaceQty and DomTessFaceNb
/// 
/// Uses FaceDom
///
extern void neut_geo_init_domtessface (struct GEO* pGeo);

/// \brief Initialize DomTessEdgeQty and DomTessEdgeNb
/// 
/// Requirements: uses DomEdgeFaceNb, DomTessFaceQty and DomTessFaceNb
///
extern void neut_geo_init_domtessedge (struct GEO* pGeo);

/// \brief Initialize DomTessVerNb
/// 
/// Uses VerDom
///
extern void neut_geo_init_domtessver (struct GEO* pGeo);

/// \brief Initialize the domain of a GEO
/// 
/// Input requirements: pointer to a GEO structure whose all non-domain
/// variables have been initialized.
/// 
/// Output: pointer to a GEO structure whose all *Dom* parameters are initialized.
///
extern void neut_geo_init_domain (struct GEO* pGeo);

/// \brief Initialize the domain of a GEO using a POLY
/// 
///
///
extern void neut_geo_init_domain_poly (struct GEO* pGeo, struct POLY Domain, char* type);

/// \brief Initialize the domain of a GEO using a POLY: simple copy, no initialization of the Geo to Domain variables (*Dom and DomTess*)
/// 
///
///
extern void neut_geo_init_domain_memcpy (struct GEO* pGeo, struct POLY Domain, char* type);

/// \brief Initialize the face labels of the domain of a GEO.
///
///
///
extern void neut_geo_init_domain_facelabel (struct GEO* pGeo);

#endif /* NEUT_GEO_OP_H */
