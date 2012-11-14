/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void neut_mesh_renumber_continuous (struct MESH*, int*, int*, int*);

// extern void neut_mesh_renumber_bypoly (int*, struct NODES*, struct MESH*, struct MESH*);
// extern void neut_mesh_renumber_crude (struct NODES*, struct MESH*, struct MESH*);
// extern void neut_mesh_renumber_partition (struct NODES*, struct MESH*, struct MESH*, int);
extern void neut_nodes_renumber (struct NODES *, int *);
extern void neut_mesh_renumber (struct MESH*, int*, int*, int*);
extern void neut_nodes_sortbynumber (struct NODES *, int*);
extern void neut_mesh_sortbynumber (struct MESH *, int*);
extern void neut_nodes_renumber_switch (struct NODES*, int*);
extern void neut_nodes_renumber_switch_mesh (struct MESH*, int*);

extern void neut_mesh_renumber_switch (struct MESH*, int*);
