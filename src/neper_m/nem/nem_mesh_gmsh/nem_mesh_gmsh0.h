/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nem_mesh_gmsh_options (FILE *, int, int, int, double);

extern void nem_mesh_2d_gmsh_writenodes (struct GEO, struct NODES,
					  struct MESH, int, double *, FILE *);
extern void nem_mesh_2d_gmsh_write1dmesh (struct GEO, struct MESH, int,
					   FILE *);
extern int nem_mesh_2d_gmsh (struct GEO, int, double *, struct NODES,
			      struct MESH, double, char *, char *, double,
			      double, struct NODES *, struct MESH *,
			      double*, double *);
extern int nem_mesh_2d_gmsh_b (struct GEO, int, struct NODES, struct MESH,
				char *, char *, double, double,
				struct NODES *, struct MESH *, double *);
extern int nem_mesh_3d_gmsh (struct GEO, int, struct NODES, struct
    MESH, double, double, char *, char *, char *, double, double, struct NODES
    *, struct MESH *, double *, double*);
extern void nem_mesh_2d_gmsh_writeface (struct GEO, struct MESH, int, double,
					 FILE *);
extern void nem_mesh_2d_gmsh_writeface_b (double, FILE *);
extern void nem_mesh_3d_gmsh_writepoly (double, FILE *);
