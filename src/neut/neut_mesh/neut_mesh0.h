/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef UT_MESH0_H
#define UT_MESH0_H

#include"../neut_structs.h"
#include "neut_mesh_renumber/neut_mesh_renumber0.h"
#include "fscanf_msh/fscanf_msh0.h"

#ifdef HAVE_LIBSCOTCH
#include <scotch.h>
extern void neut_mesh_scotchmesh (struct MESH, int, SCOTCH_Mesh*);
#endif

extern void neut_mesh_fscanf_geof (FILE *, struct NODES *, struct MESH *);
extern void neut_elt_name_prop (char *, char *, char *, int *, int *);
extern void neut_node_elts (struct MESH, int, int *, int *);
extern void neut_nodes_commonelts (struct MESH, int *, int, int *, int *);
extern void neut_elts_commonnodes (struct MESH, int *, int, int *, int *);
extern void neut_nodes_allelts (struct MESH, int *, int, int *, int *);
extern int neut_nodes_elt (struct MESH, int *, int, int *);
extern int neut_mesh_eltelsetOLD (struct MESH, int, int *);
extern void neut_mesh_eltcentre (struct MESH, struct NODES, int, double *);
extern int neut_fod_facefod (int **, int, int *);
extern int neut_mesh_elt0delts1d (struct MESH, int, struct MESH, int*, int*);
extern int neut_mesh_elt0delsets1d (struct MESH, int, struct MESH, int*, int*);
extern int neut_mesh_elt1delts2d (struct MESH, int, struct MESH, int*, int*);
extern int neut_mesh_elt1delsets2d (struct MESH, int, struct MESH, int*, int*);
extern int neut_mesh_elt2delts3d (struct MESH, int, struct MESH, int*, int*);
extern int neut_mesh_elt1delts3d (struct MESH, int, struct MESH, struct MESH, int*, int*);
extern int neut_mesh_elt0delts3d (struct MESH, int, struct MESH, struct MESH, struct MESH, int*, int*);

extern void neut_mesh_elt_reversenodes (struct MESH*, int);
extern void neut_mesh_eltnormal (struct MESH, struct NODES, int, double *);
extern void neut_mesh_elteq (struct MESH, struct NODES, int, double *);
extern void neut_mesh_set_zero (struct MESH *);
extern void neut_mesh_fscanf_fev (FILE *, FILE *, FILE *, struct NODES *,
				 struct MESH *);
extern int neut_mesh_elt_elset (struct MESH, int);
extern void neut_mesh_centre (struct NODES, struct MESH, double *);
extern void neut_mesh_elset_nodes (struct MESH, int, int **, int *);

extern void neut_mesh_boxelts (struct MESH, struct NODES, double **, int *);

extern void neut_mesh_free (struct MESH *);

extern void neut_mesh_init_nodeelts (struct MESH*, int);
extern void neut_mesh_init_eltelset (struct MESH*, int*);
extern void neut_mesh_init_elsets (struct MESH*);

extern int neut_mesh_elt_partqty (struct MESH, int, int*);
extern int neut_mesh_elt_isbound (struct MESH, int, int*);
extern int neut_mesh_elt_comqty (struct MESH, int, int *, int *);
extern int neut_mesh_elt_coms (struct MESH, int, int, int *, int *, int **,
			       int *);
extern void neut_mesh_fscanf_stellar (FILE*, FILE*, struct NODES*, struct MESH*);
extern void neut_mesh_elt_vermesh (struct MESH, int, struct MESH*);
extern void neut_mesh_elt_edgemesh (struct MESH, int, struct MESH*);
extern void neut_mesh_elt_facetmesh (struct MESH, int, struct MESH*);
extern struct MESH neut_mesh_alloc (int, char*, int, int, int);

extern void neut_mesh3d_mesh2d (struct NODES, struct MESH, struct MESH*, int***, int*, int);
extern void neut_mesh2d_mesh1d (struct NODES, struct MESH, struct MESH*, int***, int**, int*, int);
extern void neut_mesh1d_mesh0d (struct NODES, struct MESH, struct MESH*, int***, int**, int*, int);

extern void neut_mesh3d_mesh2d_facepoly (struct NODES, struct MESH, struct MESH, int**);
extern void neut_mesh_addelt (struct MESH*, int*);
extern void neut_mesh_addelset (struct MESH*, int*, int);
extern void neut_mesh_elset_addelt (struct MESH*, int, int);
extern int  neut_mesh_skinelt_fod (struct NODES, struct MESH, int, int*);
extern int  neut_mesh_skinelt_fod_b (struct NODES, struct MESH, int, double***, int*, int*, int*);

extern void neut_skinelsets_fod (int**, int, int***);

extern int neut_mesh_eltlength (struct NODES, struct MESH, int, double*);
extern void neut_mesh_order1to2 (struct NODES*, struct MESH*, struct MESH*, struct MESH*);
extern void neut_mesh_elsets_nodes (struct MESH, int*, int, int**, int*);
extern int neut_mesh_volume (struct NODES, struct MESH, double*, double*, double*);
extern int neut_mesh_elset_volume (struct NODES, struct MESH, int, double*);
extern int neut_mesh_3d_elt_rr (struct NODES, struct MESH, int, double*);
extern int neut_mesh_elset_3d_rr (struct NODES, struct MESH, int, double*, double*, double*);
extern int neut_mesh_3d_rr (struct NODES, struct MESH, double*, double*, double*);
extern int neut_mesh_3d_rr2 (struct NODES, struct MESH, char*, double*);
extern int neut_mesh_3d_rr2_array (double*, int, char*, double*);

extern int neut_mesh_2d_rr (struct NODES, struct MESH, double*, double*, double*);
extern void neut_mesh_mesh (struct MESH, struct MESH*);
extern void neut_mesh_poly_skinmesh (struct GEO, int, struct MESH, struct MESH*);
extern void neut_mesh_face_skinmesh (struct GEO, int, struct MESH, struct MESH*);
extern void neut_mesh_mergeelsets (struct MESH*);
extern int neut_mesh_mesh_match (struct NODES, struct MESH, struct NODES, struct MESH);
extern int neut_mesh_elt_volume (struct NODES, struct MESH, int, double*);
extern int neut_mesh_elset_centre (struct NODES, struct MESH, int, double*);
extern int neut_mesh_elset_averagecl (struct MESH, struct NODES, int, double*);
extern int neut_mesh_face_overrefinement (struct GEO, int, struct NODES,
    struct MESH, struct NODES, struct MESH);

extern int neut_mesh_point_elt (struct MESH, struct NODES, double*, int*);
extern int neut_mesh_elset_point_elt (struct MESH, struct NODES, int, double*, int*);
extern void neut_mesh_point_elt_in (struct MESH, struct NODES, double*, int, int*);
extern int neut_mesh_elt_area (struct NODES, struct MESH, int, double*);
extern void neut_mesh_elt3d_elts2d (struct NODES, struct MESH, int, struct MESH, int*, int*);
extern void neut_mesh_elts_elsets (struct MESH, int*, int, int*, int*);
extern void neut_mesh_elts_to_elsets (struct MESH, int*, int, int*, int*, int**, int*);
extern int neut_mesh_elset_point_closestelt (struct MESH, struct NODES, int, double*, int*);
extern void neut_mesh_centre (struct NODES, struct MESH, double*);


extern void neut_mesh_facepoly (struct NODES, struct MESH, struct MESH, int***);
extern void neut_mesh_edgeface (struct MESH, struct MESH, int***, int**);
extern void neut_mesh_veredge (struct MESH, struct MESH, int***, int**);

extern void neut_mesh_elt_lengths (struct MESH, struct NODES, int, double*, double*, double*);

extern int neut_mesh_elt_neighelts (struct MESH, int, int*, int*);
extern void neut_mesh_elts_skinelts (struct MESH, int*, int, int**, int*);

extern void neut_mesh_face_boundnodes (struct GEO, int, struct MESH, int**, int*);
extern void neut_mesh_face_boundnodes2 (struct GEO, int, struct MESH, int**, int*);
extern void neut_mesh_elset_mesh (struct NODES, struct MESH, int, struct NODES*, struct MESH*);

extern int neut_nodes_elsets_commonelts (struct MESH, int*, int, int*, int, int*, int*);

extern int neut_mesh_elset_meancl (struct NODES, struct MESH, int, double*);
extern int neut_mesh_meancl (struct NODES, struct MESH, double*);
extern int neut_mesh_dispfromcl (struct NODES, struct MESH, double, double*);
extern int neut_mesh_elset_dispfromcl (struct NODES, struct MESH, int, double, double*);

extern void rcl2cl (double, double, int, double*);
extern void rsel2sel (double, double, int, double*);

extern void neut_mesh_fprintf_gmsh (FILE*, char*, struct NODES, struct
    MESH, struct MESH, struct MESH, struct MESH,
    struct PART);

extern void neut_meshheader_fprintf_gmsh (FILE*);
extern void neut_elts_fprintf_gmsh (FILE*, struct MESH, struct MESH, struct MESH, struct MESH, struct PART, char*);
extern void neut_mesh_fprintf_gmsh_nsets (FILE*, char*, int**, char**);
extern void neut_mesh_fprintf_gmsh_nset  (FILE*, char*, int*);

extern void neut_mesh_scale (struct MESH*, double, double, double);
extern void neut_mesh_shift (struct MESH*, double, double, double);

extern void neut_mesh_set_elttype (struct MESH*, char*);

extern void neut_mesh3d_slice (struct NODES, struct MESH, double*,
			  struct NODES*, struct MESH*, int**, int***, double**);
extern int neut_mesh3d_elt_slice_tet (struct NODES Nodes, struct MESH Mesh, int elt,
                           double* eq, struct NODES* pSNodes, struct MESH* pSMesh,
			   int** pelt_newold, int*** pnode_newold, double** pnode_fact);
extern int neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH Mesh, int elt, 
			   double* eq, struct NODES* pSNodes, struct MESH* pSMesh,
			   int** pelt_newold, int*** pnode_newold, double** pnode_fact);

extern void neut_meshdata_mesh2slice (struct NODES, struct MESH, struct
    MESHDATA, struct NODES, struct MESH, int*, int**, double*, struct
    MESHDATA*);

extern int  neut_mesh_elt_point_pos (struct MESH, struct NODES, int,
				     double*, double*);

extern void neut_mesh_elt_bbox_v (struct NODES Nodes, struct MESH Mesh, int elt, double* bbox);
extern void neut_mesh_elt_bbox (struct NODES Nodes, struct MESH Mesh, int elt, double** bbox);
extern void neut_mesh_elt_poly (struct NODES Nodes, struct MESH Mesh, int elt, struct POLY* pPoly);
extern int neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh, int elt,
                    struct NODES* pENodes, struct MESH* pEMesh);
extern void neut_mesh_quad_tri (struct NODES, struct MESH, struct MESH*);
extern int neut_mesh_rmelset (struct MESH*, struct NODES, int);
extern int neut_mesh_rmelt (struct MESH*, int);
extern int neut_mesh_rmelts (struct MESH* pMesh, struct NODES Nodes, int* rmelt, int rmeltqty);

#endif /* UT_MESH0_H */

