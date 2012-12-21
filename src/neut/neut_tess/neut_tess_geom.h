/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_TESS_GEOM_H
#define NEUT_TESS_GEOM_H

#include"../neut_structs.h"

/// \brief Get the centre of an edge
///
///
///
extern int neut_tess_edge_centre (struct TESS Tess, int, double*);

/// \brief Get the area of a face
///
///
///
extern int neut_tess_face_area (struct TESS Tess, int, double*);

/// \brief Get the centre of a face
///
///
///
extern void neut_tess_face_centre (struct TESS Tess, int, double*);

/// \brief Get the volume of a polyhedron
///
///
///
extern int neut_tess_poly_volume (struct TESS Tess, int, double*);

/// \brief Get the centroid of a polyhedron
///
///
///
extern int neut_tess_poly_centroid (struct TESS Tess, int, double*);

/// \brief Get the volume of a TESS
///
///
///
extern int neut_tess_volume (struct TESS Tess, double*);

/// \brief Get the centre of a TESS
///
///
///
extern void neut_tess_centre (struct TESS Tess, double*);

/// \brief Get the bounding box of a TESS
///
///
///
extern void neut_tess_bbox (struct TESS Tess, double**);

/// \brief Determine whether a point is within a polyhedron 
///
///
///
extern int neut_tess_point_inpoly (struct TESS Tess, double*, int);

extern int neut_tess_edge_selratio (struct TESS Tess, struct REG, int, double*);
extern void rcl2cl (double, double, int, char*, double*);
extern void rsel2sel (double, double, int, double*);

#endif /* NEUT_TESS_GEOM_H */
