/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_DATA0_H
#define NEUT_DATA0_H

extern void neut_tessdata_set_default  (struct TESSDATA*);
extern void neut_meshdata_set_default  (struct MESHDATA*);

extern int neut_meshdata_entity_id (char*, int*);
extern int neut_meshdata_id_entity (int, char*);
extern int neut_meshdata_id_qty (struct MESHDATA, int, int*);
extern int neut_tessdata_idmax (int*);
extern int neut_meshdata_idmax (int*);
extern int neut_meshdata_entity_qty (struct MESHDATA, char*, int*);
extern int neut_meshdata_type_qty (char*, int*);

extern void neut_tessdata_free (struct TESSDATA*);
extern void neut_meshdata_free (struct MESHDATA*);
extern int neut_tessdata_entity_id (char*, int*);
extern int neut_tessdata_id_entity (int, char*);
extern int neut_tessdata_id_qty (struct TESSDATA, int, int*);

extern void neut_meshdata_mesh2slice (struct NODES Nodes, struct MESH
    Mesh, struct MESHDATA MeshData, struct NODES SNodes, struct MESH
    SMesh, int* elt_newold, int** node_newold, double* node_fact, struct
    MESHDATA* pSMeshData);

#endif /* NEUT_DATA0_H */
