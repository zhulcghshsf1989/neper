/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void nefm_mesh_fprintf_stellar_config (FILE*);
extern void nefm_mesh_3d_improve_stellar (struct NODES*, struct MESH*, int);
extern void nefm_mesh_fprintf_stellar_nodes (FILE*, int*, struct NODES);
extern void nefm_mesh_fprintf_stellar_elts (FILE*, struct MESH);
extern void WriteMeshStellar (struct IN, struct NODES, int*, struct MESH);
