/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_SPACE_H
#define UT_SPACE_H

#include "ut_config.h"

extern double ut_space_dist (double *, double *);
extern double ut_space_dist_int (int *, int *);
extern double ut_space_dist2d (double *, double *);
extern int ut_space_quad_convex (double *, double *, double *, double *);
extern void ut_space_bary2d (double **, int, double *);
extern int ut_space_quad_convex_int (int *, int *, int *, int *);
extern void ut_space_bary2d_int (int **, int, int *);
extern int ut_space_indomain2d (double **, int, double, double);
extern int ut_space_indomain2d_int (int **, int, int *);

extern int ut_space_planeside (double *, double *);
extern int ut_space_planeside_tol (double *, double *, double);
extern int ut_space_planeside_2d (double *, double *);

extern double ut_space_trianglearea (double *, double *, double *);
extern void ut_space_trianglenormal (double *, double *, double *, double *);
extern int ut_space_pointaligned (double *, double *, double *, double);

extern void ut_space_arrangepoints (double **, int, double **, int, int *);

extern double ut_space_triangle_area (double *, double *, double *);
extern int ut_space_tet_centre (double *, double *, double *, double *, double*);
extern double ut_space_tet_volume (double *, double *, double *, double *);
extern int ut_space_hexa_center (double*, double*, double*, double*,
				 double*, double*, double*, double*,
				 double*);
extern double ut_space_hexa_volume (double*, double*, double*, double*,
                      double*, double*, double*, double*);
#ifdef HAVE_GSL
extern void ut_space_projpoint_alongonto (double *, double *, double *);
extern int ut_space_point_plane_dist (double*, double*, double*);
extern int ut_space_point_plane_dist_signed (double*, double*, double*);
extern int ut_space_segment_plane_intersect (double*, double*, double*, double*);
#endif

extern void ut_space_points_invect_plane (double *, double *, double *,
					  double *);
extern void ut_space_points_plane (double *, double *, double *, double *);
extern int ut_space_triangle_point_in (double *, double *, double *,
				       double *);
extern int ut_space_tet_point_in (double *, double *, double *, double *,
				  double *);

extern int ut_space_segments_intersect (double*, double*, double*, double*);

extern void ut_space_points_covarmatrix (double**, double*, int, int, double**, double*);

extern int ut_space_points_aligned (double*, double*, double*);
extern int ut_space_points_aligned2 (double*, double*, double*);
extern int ut_space_planes_equal (double*, double*);

extern int ut_space_segment_point_in (double*, double*, double*);

extern int ut_space_contour_intersect (double**, int);

extern int ut_space_points_uvect (double*, double*, double*);


extern void ut_space_string_plane (char*, double*);

#endif /* UT_SPACE_H */
