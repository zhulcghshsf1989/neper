/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_meshing.h"

void
nem_mesh_pinching (struct GEO Geo, double** face_eq, struct MESH
    Mesh1D, struct NODES Nodes, struct MESH* pMesh2D)
{
  int i, j, k;
  int edge, face, elt1d, pos;
  int toredo;
  int polyedgeqty;
  int* polyedge = NULL;
  int* edgeface = NULL;
  int* elts2d = NULL;
  int elt2dqty;
  double* n1 = ut_alloc_1d (3);
  double* n2 = ut_alloc_1d (3);
  double* n1vn2 = ut_alloc_1d (3);
  double n1n2angle;
  double* edge_v = ut_alloc_1d (3);
  double* elt1d_v = ut_alloc_1d (3);

  ut_print_message (0, 2, "Checking 2D mesh for pinching out ...\n");

  // for every polyhedron, checking if it is pinched out somewhere.
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    neut_geo_poly_edges (Geo, i, &polyedge, &polyedgeqty);

    // for every edge, looking for its elements in its two faces.
    for (j = 0; j < polyedgeqty; j++)
    {
      edge = polyedge[j];
      neut_geo_poly_edge_faces (Geo, i, edge, &edgeface);
      // determining a signed axis for the edge
      ut_vector_vectprod (face_eq[edgeface[0]], face_eq[edgeface[1]], n1);
      ut_vector_uvect (n1, edge_v);

      toredo = 0;
      for (k = 1; k <= Mesh1D.Elsets[edge][0]; k++)
      {
	elt1d = Mesh1D.Elsets[edge][k]; 
	neut_mesh_nodes_elsets_comelts (*pMesh2D,
	    Mesh1D.EltNodes[elt1d], 2, edgeface, 2, &elts2d, &elt2dqty);
	if (elt2dqty != 2)
	  ut_error_reportbug ();

	ut_space_points_uvect (
	    Nodes.NodeCoo[Mesh1D.EltNodes[elt1d][0]], 
	    Nodes.NodeCoo[Mesh1D.EltNodes[elt1d][1]], 
	    elt1d_v);
	if (ut_vector_scalprod (edge_v, elt1d_v) < 0)
	  ut_array_1d_scale (elt1d_v, 3, -1);

	// getting the elt normal, outgoing 
	neut_mesh_elt_normal (*pMesh2D, Nodes, elts2d[0], n1);
	face = (*pMesh2D).EltElset[elts2d[0]];
	pos = 1 + ut_array_1d_int_eltpos (Geo.PolyFaceNb[i] + 1,
				  Geo.PolyFaceQty[i], face);
	ut_array_1d_scale (n1, 3, (double) Geo.PolyFaceOri[i][pos]);

	// getting the elt normal, outgoing 
	neut_mesh_elt_normal (*pMesh2D, Nodes, elts2d[1], n2);
	face = (*pMesh2D).EltElset[elts2d[1]];
	pos = 1 + ut_array_1d_int_eltpos (Geo.PolyFaceNb[i] + 1,
				  Geo.PolyFaceQty[i], face);
	ut_array_1d_scale (n2, 3, (double) Geo.PolyFaceOri[i][pos]);

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
	    nem_remesh_fixmesh2delset (Geo, Mesh1D, edgeface[0], pMesh2D);
	    nem_remesh_fixmesh2delset (Geo, Mesh1D, edgeface[1], pMesh2D);
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

  ut_free_1d_int (polyedge);
  ut_free_1d_int (edgeface);
  ut_free_1d_int (elts2d);
  ut_free_1d (n1);
  ut_free_1d (n2);
  ut_free_1d (n1vn2);
  ut_free_1d (edge_v);
  ut_free_1d (elt1d_v);

  return;
}
