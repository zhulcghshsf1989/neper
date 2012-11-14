/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nevs_print_segment (FILE*, double*, double*, char*, char*);
extern void nevs_print_segment_wsph (FILE*, double*, double*, char*, char*);
extern void nevs_print_sphere (FILE*, double*, char*, char*);
extern void nevs_print_triangle_edge (FILE*, double*, double*, double*, char*, char*);
extern void nevs_print_triangle (FILE*, double*, double*, double*, char*, char*, char*);
extern void nevs_print_polygon (FILE*, int, double**, char*, char*, char*, double*, char*, char*);
extern void nevs_print_polygon_edge (FILE*, int, double**, char*, char*);

extern void nevs_print_tet_edge (FILE*, double*, double*, double*, double*, char*, char*);
extern void nevs_print_tet (FILE*, double*, double*, double*, double*, char*, char*, char*);

extern void nevs_print_mesh2d (FILE*, struct NODES, struct MESH, struct PRINT, int**, char*);

extern void nevs_print_scale (FILE*, char*, char*, char*, char*);
