/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_gen_lcl.h"

int
neut_mesh_point_elt (struct MESH Mesh, struct NODES Nodes, 
    double* coo, int* pelt)
{
  int i, status;

  status = 1;
  for (i = 1; i <= Mesh.ElsetQty; i++)
  {
    status = neut_mesh_elset_point_elt (Mesh, Nodes, i, coo, pelt);

    if (status == 0)
      break;
  }

  return status;
}

int
neut_mesh_elset_point_elt (struct MESH Mesh, struct NODES Nodes, int elset,
    double* coo, int* pelt)
{
  int i, inelt;

  inelt = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    (*pelt) = Mesh.Elsets[elset][i];

    neut_mesh_point_elt_in (Mesh, Nodes, coo, (*pelt), &inelt);

    if (inelt == 1)
      break;
  }

  return (inelt == 1) ? 0 : 1;
}

int
neut_mesh_elset_point_closestelt (struct MESH Mesh, struct NODES Nodes,
    int elset, double* coo, int* pelt)
{
  int i, elt, id;
  double* dist = ut_alloc_1d (Mesh.Elsets[elset][0]);
  double* eltcoo = ut_alloc_1d (3);

  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];
    neut_mesh_elt_centre (Mesh, Nodes, elt, eltcoo);
    dist[i - 1] = ut_space_dist (coo, eltcoo);
  }
  id = 1 + ut_array_1d_min_index (dist, Mesh.Elsets[elset][0]);
  
  (*pelt) = Mesh.Elsets[elset][id];

  ut_free_1d (dist);
  ut_free_1d (eltcoo);

  return 0;
}

int
neut_mesh_point_proj_alongonto (double* Coo, double* n, struct NODES N, struct MESH M, int elset)
{
  int j, elt, status;
  double* coo = ut_alloc_1d (3);
  double* eq = ut_alloc_1d (4);

  // FILE* file;
  // struct PART PGarbage;
  // sprintf (message, "face-%d-bef.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");
  
  status = -1;
  
  for (j = 1; j <= M.Elsets[elset][0]; j++)
  {
    elt = M.Elsets[elset][j];
    ut_array_1d_memcpy (coo, 3, Coo);
    neut_mesh_elt_eq (M, N, elt, eq);
    ut_space_projpoint_alongonto (coo, n, eq);

    if (ut_space_triangle_point_in (N.NodeCoo[M.EltNodes[elt][0]],
	  N.NodeCoo[M.EltNodes[elt][1]], N.NodeCoo[M.EltNodes[elt][2]], coo) == 1)
    {
      ut_array_1d_memcpy (Coo, 3, coo);
      status = elt;
      break;
    }
  }

  ut_free_1d (coo);
  ut_free_1d (eq);
  
  // sprintf (message, "face-%d-aft.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  return status;
}

void
neut_mesh_point_elt_in (struct MESH Mesh, struct NODES Nodes,
    double* coo, int elt, int* pinelt)
{

  (*pinelt) =
    ut_space_tet_point_in (Nodes.NodeCoo[Mesh.EltNodes[elt][0]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][1]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][2]],
                           Nodes.NodeCoo[Mesh.EltNodes[elt][3]], coo);

  return;
}

void
neut_mesh3d_slice (struct NODES Nodes, struct MESH Mesh, double* eq,
		   struct NODES* pSNodes, struct MESH* pSMesh,
	           int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i;

  if (Mesh.Dimension != 3)
    ut_error_reportbug ();

  (*pSMesh).Dimension = 2;
  (*pSMesh).EltOrder = 1;
  (*pSMesh).EltType = ut_alloc_1d_char (4);
  strcpy ((*pSMesh).EltType, "tri");

  (*pelt_newold)  = ut_alloc_1d_int (1);
  (*pnode_newold) = ut_alloc_2d_int (1, 2);
  (*pnode_fact)   = ut_alloc_1d     (1);
  
  if (! strcmp (Mesh.EltType, "tri"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_tet (Nodes, Mesh, i, eq, pSNodes, pSMesh, pelt_newold,
				 pnode_newold, pnode_fact);
  else if (! strcmp (Mesh.EltType, "quad"))
    for (i = 1; i <= Mesh.EltQty; i++)
      neut_mesh3d_elt_slice_quad (Nodes, Mesh, i, eq, pSNodes, pSMesh, pelt_newold,
				  pnode_newold, pnode_fact);
  else
    ut_error_reportbug ();

  neut_mesh_addelset (pSMesh, NULL, (*pSMesh).EltQty);

  return;
}

int
neut_mesh3d_elt_slice_tet (struct NODES Nodes, struct MESH Mesh, int elt, double* eq,
			   struct NODES* pSNodes, struct MESH* pSMesh,
			   int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i, j, node;
  int* side = ut_alloc_1d_int (4);
  int siden, intertype, nodemaster, id;
  int* pos = ut_alloc_1d_int (2);
  int* pos2 = ut_alloc_1d_int (2);
  double* coo = ut_alloc_1d (3);
  int* eltnodes = ut_alloc_1d_int (3);
  double dist, distmaster;

  // intertype = 0 -> no intersection
  //
  // intertype = 1 -> 1|3 vertices (two sides of the plane): the
  // intersection between the elt and the plane is a triangle.
  //
  // intertype = 2 -> 2|2 vertices (two sides of the plane): the 
  // intersection between the elt and the plane is a quadrangle;
  // spliting it into two triangles.

  for (i = 0; i < 4; i++)
  {
    node = Mesh.EltNodes[elt][i];
    side[i] = ut_space_planeside (eq, Nodes.NodeCoo[node] - 1);
  }
  siden = ut_array_1d_int_nbofthisval (side, 4, -1);
  intertype = ut_num_min (siden, 4 - siden);

  if (intertype == 1)
  {
    // finding master node
    if (siden == 1)
      pos[0] = ut_array_1d_int_eltpos (side, 4, -1);
    else if (siden == 3)
      pos[0] = ut_array_1d_int_eltpos (side, 4,  1);
    else
      ut_error_reportbug ();

    nodemaster = Mesh.EltNodes[elt][pos[0]];
    ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq, 
			       &distmaster);

    // finding intersection between the tet and the plane (triangle)
    id = 0;
    for (i = 0; i < 4; i++)
      if (i != pos[0])
      {
	node = Mesh.EltNodes[elt][i];
	ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
	    Nodes.NodeCoo[node], eq, coo);
	neut_nodes_addnode (pSNodes, coo, 0);
	
	ut_space_point_plane_dist (Nodes.NodeCoo[node], eq, &dist);
	  
	eltnodes[id++] = (*pSNodes).NodeQty;

	(*pnode_newold)  = ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1, 2);
	(*pnode_fact)    = ut_realloc_1d             ((*pnode_fact)  , (*pSNodes).NodeQty + 1);
    
	(*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
	(*pnode_newold)[(*pSNodes).NodeQty][1] = node;
	(*pnode_fact)  [(*pSNodes).NodeQty]    = distmaster / (distmaster + dist);
      }

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }

  else if (intertype == 2)
  {
    int id = 0;
    int id2 = 0;
    for (i = 0; i < 4; i++)
      if (side[i] == -1)
	pos[id++] = i;
      else
	pos2[id2++] = i;

    // Finding intersection between the tet and the plane
    // (quadrangle).  Splitting up the quadrangle into two triangles
    // for plotting. 
    
    for (j = 0; j < 2; j++)
    {
      nodemaster = Mesh.EltNodes[elt][pos[j]];
      ut_space_point_plane_dist (Nodes.NodeCoo[nodemaster], eq, 
				 &distmaster);

      for (i = 0; i < 2; i++)
      {
	node = Mesh.EltNodes[elt][pos2[i]];
	ut_space_point_plane_dist (Nodes.NodeCoo[node], eq,
				   &dist);

	ut_space_segment_plane_intersect (Nodes.NodeCoo[nodemaster],
	    Nodes.NodeCoo[node], eq, coo);
	neut_nodes_addnode (pSNodes, coo, 0);

	(*pnode_newold)  = ut_realloc_2d_int_addline ((*pnode_newold), (*pSNodes).NodeQty + 1, 2);
	(*pnode_fact)    = ut_realloc_1d             ((*pnode_fact)  , (*pSNodes).NodeQty + 1);
    
	(*pnode_newold)[(*pSNodes).NodeQty][0] = nodemaster;
	(*pnode_newold)[(*pSNodes).NodeQty][1] = node;
	(*pnode_fact)  [(*pSNodes).NodeQty]    = distmaster / (distmaster + dist);
      }
    }
    
    eltnodes[0] = (*pSNodes).NodeQty - 3;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty - 1;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;

    eltnodes[0] = (*pSNodes).NodeQty - 1;
    eltnodes[1] = (*pSNodes).NodeQty - 2;
    eltnodes[2] = (*pSNodes).NodeQty;

    neut_mesh_addelt (pSMesh, eltnodes);

    (*pelt_newold) = ut_realloc_1d_int ((*pelt_newold), (*pSMesh).EltQty + 1);
    (*pelt_newold)[(*pSMesh).EltQty] = elt;
  }
  
  ut_free_1d_int (side);
  ut_free_1d     (coo);
  ut_free_1d_int (pos);
  ut_free_1d_int (pos2);
  ut_free_1d_int (eltnodes);

  return intertype;
}

int
neut_mesh3d_elt_slice_quad (struct NODES Nodes, struct MESH Mesh, int elt, double* eq,
			    struct NODES* pSNodes, struct MESH* pSMesh,
			    int** pelt_newold, int*** pnode_newold, double** pnode_fact)
{
  int i, j, sl_elt, sl_node, eltqty, nodeqty;
  struct NODES ENodes;
  struct  MESH EMesh;
  struct  MESH TEMesh;

  neut_nodes_set_zero (&ENodes);
  neut_mesh_set_zero (&EMesh);
  neut_mesh_set_zero (&TEMesh);

  neut_mesh_elt_mesh (Nodes, Mesh, elt, &ENodes, &EMesh);
  
  neut_mesh_quad_tri (ENodes, EMesh, &TEMesh);
  
  for (i = 1; i <= 6; i++)
  {
    nodeqty = (*pSNodes).NodeQty;
    eltqty  = (*pSMesh).EltQty;
    neut_mesh3d_elt_slice_tet (ENodes, TEMesh, i, eq, pSNodes, pSMesh,
			       pelt_newold, pnode_newold, pnode_fact);

    // Renumbering added elts
    for (sl_elt = eltqty + 1; sl_elt <= (*pSMesh).EltQty; sl_elt++)
      (*pelt_newold)[sl_elt] = elt;

    // Renumbering added nodes
    for (sl_node = nodeqty + 1; sl_node <= (*pSNodes).NodeQty; sl_node++)
      for (j = 0; j < 2; j++)
	(*pnode_newold)[sl_node][j] = Mesh.EltNodes[elt][(*pnode_newold)[sl_node][j] - 1];
  }
  
  neut_nodes_free (&ENodes);
  neut_mesh_free (&EMesh);
  neut_mesh_free (&TEMesh);

  return 0;
}


int
neut_mesh_elt_mesh (struct NODES Nodes, struct MESH Mesh, int elt,
                    struct NODES* pENodes, struct MESH* pEMesh)
{
  int i, node;
  int eltnodeqty = neut_elt_nodeqty (Mesh.EltType, Mesh.Dimension, Mesh.EltOrder);

  // Setting *pENodes
  neut_nodes_set_zero (pENodes);

  (*pENodes).NodeQty = eltnodeqty;

  (*pENodes).NodeCoo = ut_alloc_2d ((*pENodes).NodeQty + 1, 3);

  for (i = 1; i <= eltnodeqty; i++)
  {
    node = Mesh.EltNodes[elt][i - 1];
    ut_array_1d_memcpy ((*pENodes).NodeCoo[i], 3, Nodes.NodeCoo[node]);
  }

  // Setting *pEMesh
  neut_mesh_set_zero (pEMesh);

  (*pEMesh).EltType = ut_alloc_1d_char (strlen (Mesh.EltType) + 1);
  strcpy ((*pEMesh).EltType, Mesh.EltType);

  (*pEMesh).Dimension = Mesh.Dimension;
  (*pEMesh).EltOrder = Mesh.EltOrder;

  (*pEMesh).EltQty = 1;
  (*pEMesh).EltNodes = ut_alloc_2d_int (2, eltnodeqty);

  for (i = 0; i < eltnodeqty; i++)
    (*pEMesh).EltNodes[1][i] = i + 1;

  return 0;
}


void
neut_mesh_elset_mesh (struct NODES Nodes, struct MESH Mesh,
                      int elset, struct NODES* pENodes, struct MESH* pEMesh)
{
  int i, j, node, elt, eltnodeqty;
  double l;
  int* node_nbs = NULL;
  int* node_nbs_inv = NULL;

  neut_nodes_set_zero (pENodes);
  neut_mesh_set_zero (pEMesh);
  
  (*pEMesh).Dimension = Mesh.Dimension;
  (*pEMesh).EltOrder = 1;
  eltnodeqty = neut_elt_nodeqty ((*pEMesh).EltType, (*pEMesh).Dimension, (*pEMesh).EltOrder);

  node_nbs = ut_alloc_1d_int (eltnodeqty * Mesh.Elsets[elset][0]);

  l = 0;
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
  {
    elt = Mesh.Elsets[elset][i];

    neut_mesh_addelt (pEMesh, Mesh.EltNodes[elt]);

    for (j = 0; j < eltnodeqty; j++)
    {
      node = Mesh.EltNodes[elt][j];
      if (ut_array_1d_int_eltpos (node_nbs + 1, node_nbs[0], node) == -1)
      {
	node_nbs[++node_nbs[0]] = Mesh.EltNodes[elt][j];

	neut_mesh_elt_length (Nodes, Mesh, elt, &l);
	neut_nodes_addnode (pENodes, Nodes.NodeCoo[node], l);
      }
    }
  }

  (*pEMesh).ElsetQty = 1;
  (*pEMesh).Elsets = ut_alloc_2d_int (2, (*pEMesh).EltQty + 1);
  for (i = 1; i <= Mesh.Elsets[elset][0]; i++)
    (*pEMesh).Elsets[1][++(*pEMesh).Elsets[1][0]] = i;

  neut_mesh_init_eltelset (pEMesh, NULL);

  int qty;
  ut_array_1d_int_inv (node_nbs + 1, node_nbs[0], &node_nbs_inv, &qty);
  for (i = 1; i <= (*pEMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pEMesh).EltNodes[i][j] = node_nbs_inv[(*pEMesh).EltNodes[i][j]] + 1;

  ut_free_1d_int (node_nbs);
  ut_free_1d_int (node_nbs_inv);

  return;
}

void
neut_mesh_face_boundmesh (struct MESH Mesh1D, struct GEO Geo, int face, struct MESH* pBMesh)
{
  int e, i, j, edge, elt;
  int* nodes = ut_alloc_1d_int (3);
  int** seq = ut_alloc_2d_int (2, 2);
  int ori;
  int* elts = NULL;

  seq[0][0] = 0;
  seq[0][1] = 1;
  seq[1][0] = 1;
  seq[1][1] = 0;

  (*pBMesh).Dimension = Mesh1D.Dimension;
  (*pBMesh).EltOrder = Mesh1D.EltOrder;
  (*pBMesh).EltType = ut_alloc_1d_char (strlen (Mesh1D.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh1D.EltType);

  for (e = 1; e <= Geo.FaceVerQty[face]; e++)
  {
    edge = Geo.FaceEdgeNb[face][e];
    ori = (Geo.FaceEdgeOri[face][e] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh1D.Elsets[edge][0]; i++)
    {
      elt = Mesh1D.Elsets[edge][i];
      
      for (j = 0; j < 2; j++)
	nodes[j] = Mesh1D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pBMesh, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pBMesh).EltQty);
  for (i = 0; i < (*pBMesh).EltQty; i++)
    elts[i] = i + 1;
  
  neut_mesh_addelset (pBMesh, elts, (*pBMesh).EltQty);
  
  neut_mesh_init_eltelset (pBMesh, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (nodes);
  ut_free_1d_int (elts);
  ut_free_2d_int (seq, 2);

  return;
}

void
neut_mesh_face_boundnodes (struct MESH Mesh1D, struct GEO Geo, int face, int** pnodes, int* pnodeqty)
{
  int i, j, edge, elt;
  // note: only the 1st-order nodes are recorded
  
  (*pnodeqty) = 0;
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];
    (*pnodeqty) += Mesh1D.Elsets[edge][0];
  }

  (*pnodes) = ut_alloc_1d_int (*pnodeqty);

  (*pnodeqty) = 0;
  // for every edge
  for (i = 1; i <= Geo.FaceVerQty[face]; i++)
  {
    edge = Geo.FaceEdgeNb[face][i];

    if (Geo.FaceEdgeOri[face][i] == 1)
      for (j = 1; j <= Mesh1D.Elsets[edge][0]; j++)
      {
	elt = Mesh1D.Elsets[edge][j];
	(*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][0];
      }
    else
      for (j = Mesh1D.Elsets[edge][0]; j >= 1; j--)
      {
	elt = Mesh1D.Elsets[edge][j];
	(*pnodes)[(*pnodeqty)++] = Mesh1D.EltNodes[elt][1];
      }
  }

  return;
}

void
neut_mesh_poly_boundmesh (struct GEO Geo, int poly, struct MESH Mesh2D,
    struct MESH* pBMesh)
{
  int f, i, j, face, elt;
  int* nodes = ut_alloc_1d_int (3);
  int** seq = ut_alloc_2d_int (2, 3);
  int ori;
  int* elts = NULL;

  seq[0][0] = 0;
  seq[0][1] = 1;
  seq[0][2] = 2;
  seq[1][0] = 0;
  seq[1][1] = 2;
  seq[1][2] = 1;

  (*pBMesh).Dimension = Mesh2D.Dimension;
  (*pBMesh).EltOrder = Mesh2D.EltOrder;
  (*pBMesh).EltType = ut_alloc_1d_char (strlen (Mesh2D.EltType) + 1);
  strcpy ((*pBMesh).EltType, Mesh2D.EltType);

  for (f = 1; f <= Geo.PolyFaceQty[poly]; f++)
  {
    face = Geo.PolyFaceNb[poly][f];
    ori = (Geo.PolyFaceOri[poly][f] == 1) ? 0 : 1;

    for (i = 1; i <= Mesh2D.Elsets[face][0]; i++)
    {
      elt = Mesh2D.Elsets[face][i];
      
      for (j = 0; j < 3; j++)
	nodes[j] = Mesh2D.EltNodes[elt][seq[ori][j]];

      neut_mesh_addelt (pBMesh, nodes);
    }
  }

  elts = ut_alloc_1d_int ((*pBMesh).EltQty);
  for (i = 0; i < (*pBMesh).EltQty; i++)
    elts[i] = i + 1;
  
  neut_mesh_addelset (pBMesh, elts, (*pBMesh).EltQty);
  
  neut_mesh_init_eltelset (pBMesh, NULL);

  /*----------------------------------------------------------------------
   * foot */

  ut_free_1d_int (nodes);
  ut_free_1d_int (elts);
  ut_free_2d_int (seq, 2);

  return;
}
