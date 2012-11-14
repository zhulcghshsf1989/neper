/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Remeshing.h"

void
Remeshing (struct IN In, struct GEOPARA GeoPara, struct GEO* pGeo,
    struct NODES* pRNodes, struct MESH RMesh0D, struct MESH* pRMesh1D,
    struct MESH* pRMesh2D, struct MESH RMesh3D, struct NODES *pNodes,
    struct MESH* pMesh0D, struct MESH* pMesh1D, struct MESH* pMesh2D,
    struct MESH* pMesh3D)
{
  int i, j, k, edge, face, elt1d, pos;
  double** face_eq = ut_alloc_2d ((*pGeo).FaceQty + 1, 4);
  int* face_op = ut_alloc_1d_int ((*pGeo).FaceQty + 1);
  int* edge_op = ut_alloc_1d_int ((*pGeo).EdgeQty + 1);
  int polyedgeqty;
  int* polyedge = NULL;
  int* edgeface = NULL;
  int* elts2d = ut_alloc_1d_int (10);
  int elt2dqty;
  double* edge_v = ut_alloc_1d (3);
  double* elt1d_v = ut_alloc_1d (3);
  double* n1 = ut_alloc_1d (3);
  double* n2 = ut_alloc_1d (3);
  double* n1vn2 = ut_alloc_1d (3);
  double n1n2angle;
  int toredo;
  struct PART Part;

  neut_part_set_zero (&Part);

  Premeshing (*pGeo, pRNodes, pRMesh1D, pRMesh2D, NULL, NULL, NULL, face_eq, edge_op, face_op);

  // generating new meshes ---------------------------------------------
  Meshing0D (*pGeo, GeoPara, pNodes, pMesh0D);
  Meshing1D (*pGeo, GeoPara, pRNodes, pRMesh1D, edge_op, pNodes, pMesh1D);
  Meshing2D (In, GeoPara, *pGeo, NULL, NULL, NULL, face_eq, face_op, pRNodes, pRMesh2D, \
             *pMesh0D, *pMesh1D, pNodes, pMesh2D);
  
  neut_mesh_init_eltelset (pMesh2D, NULL);

  /*
  file = ut_file_open ("debug", "w");
  WriteMeshGmsh (file, "0,1,2", *pNodes, *pMesh0D, *pMesh1D, *pMesh2D, *pMesh3D, NULL, NULL, NULL, Part);
  ut_file_close (file, "debug", "w");
  */
  
  /*
  file = ut_file_open ("debug", "r");
  neut_mesh_fscanf_msh (file, pNodes, pMesh0D, pMesh1D, pMesh2D, NULL);
  ut_file_close (file, "debug", "r");
  */ 

  neut_mesh_init_eltelset (pMesh2D, NULL);
  neut_mesh_init_nodeelts (pMesh0D, (*pNodes).NodeQty);
  neut_mesh_init_nodeelts (pMesh1D, (*pNodes).NodeQty);
  neut_mesh_init_nodeelts (pMesh2D, (*pNodes).NodeQty);

  ut_print_message (0, 2, "Checking 2D mesh for pinching out ...\n");

  // for every polyhedron, checking if it is pinched out somewhere.
  for (i = 1; i <= (*pGeo).PolyQty; i++)
  {
    neut_geo_poly_edges (*pGeo, i, &polyedge, &polyedgeqty);

    // for every edge, looking for its elements in its two faces.
    for (j = 0; j < polyedgeqty; j++)
    {
      edge = polyedge[j];
      neut_geo_poly_edge_faces (*pGeo, i, edge, &edgeface);
      // determining a signed axis for the edge
      ut_vector_vectprod (face_eq[edgeface[0]], face_eq[edgeface[1]], n1);
      ut_vector_uvect (n1, edge_v);

      toredo = 0;
      for (k = 1; k <= (*pMesh1D).Elsets[edge][0]; k++)
      {
	elt1d = (*pMesh1D).Elsets[edge][k]; 
	neut_nodes_elsets_commonelts (*pMesh2D,
	    (*pMesh1D).EltNodes[elt1d], 2, edgeface, 2, elts2d, &elt2dqty);
	if (elt2dqty != 2)
	  ut_error_reportbug ();

	ut_space_points_uvect (
	    (*pNodes).NodeCoo[(*pMesh1D).EltNodes[elt1d][0]], 
	    (*pNodes).NodeCoo[(*pMesh1D).EltNodes[elt1d][1]], 
	    elt1d_v);
	if (ut_vector_scalprod (edge_v, elt1d_v) < 0)
	  ut_array_1d_scale (elt1d_v, 3, -1);

	// getting the elt normal, outgoing 
	neut_mesh_eltnormal (*pMesh2D, *pNodes, elts2d[0], n1);
	face = (*pMesh2D).EltElset[elts2d[0]];
	pos = 1 + ut_array_1d_int_eltpos ((*pGeo).PolyFaceNb[i] + 1,
				  (*pGeo).PolyFaceQty[i], face);
	ut_array_1d_scale (n1, 3, (double) (*pGeo).PolyFaceOri[i][pos]);

	// getting the elt normal, outgoing 
	neut_mesh_eltnormal (*pMesh2D, *pNodes, elts2d[1], n2);
	face = (*pMesh2D).EltElset[elts2d[1]];
	pos = 1 + ut_array_1d_int_eltpos ((*pGeo).PolyFaceNb[i] + 1,
				  (*pGeo).PolyFaceQty[i], face);
	ut_array_1d_scale (n2, 3, (double) (*pGeo).PolyFaceOri[i][pos]);

	ut_vector_vectprod (n1, n2, n1vn2);
	n1n2angle = ut_vector_angle (n1, n2);
	if (ut_vector_scalprod (elt1d_v, n1vn2) < 0)
	  n1n2angle = 360 - n1n2angle;

	if (n1n2angle > 177 && n1n2angle < 183)
	{
	  if (toredo == 0)
	  {
	    printf ("Pinching for poly %d (faces %d and %d)!\n", i, edgeface[0], edgeface[1]);
	    printf ("elt2d1 = %d n1 = ", elts2d[0]);
	    ut_array_1d_fprintf (stdout, n1, 3, "%f");
	    printf ("elt2d2 = %d n2 = ", elts2d[1]);
	    ut_array_1d_fprintf (stdout, n2, 3, "%f");
	    printf ("elt1d_v = ");
	    ut_array_1d_fprintf (stdout, elt1d_v, 3, "%f");
	    printf ("n1n2angle = %f\n", n1n2angle);
	    toredo = 1;
	    nefm_remesh_fixmesh2delset (*pGeo, *pMesh1D, edgeface[0], pMesh2D);
	    nefm_remesh_fixmesh2delset (*pGeo, *pMesh1D, edgeface[1], pMesh2D);
	    // k--;
	    continue;
	  }
	  else
	  {
	    printf ("Pinching not fixed\n");
	    ut_error_reportbug ();
	  }
	}
      }
    }
  }

  Meshing3D (In, GeoPara, *pGeo, pNodes, *pMesh2D, pMesh3D);

  RMesh0D.EltQty = RMesh0D.EltQty;
  RMesh3D.EltQty = RMesh3D.EltQty;
  
  ut_free_2d (face_eq, (*pGeo).FaceQty + 1);
  ut_free_1d_int (face_op);
  ut_free_1d_int (edge_op);
  ut_free_1d_int (polyedge);
  ut_free_1d_int (edgeface);
  ut_free_1d_int (elts2d);
  ut_free_1d (edge_v);
  ut_free_1d (elt1d_v);
  ut_free_1d (n1);
  ut_free_1d (n2);
  ut_free_1d (n1vn2);

  return;
}
