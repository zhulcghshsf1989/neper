/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_nodes_lcl.h"

void
neut_nodes_scale (struct NODES *pNodes, double scalex, double scaley,
		  double scalez)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).NodeCoo[i][0] *= scalex;
    (*pNodes).NodeCoo[i][1] *= scaley;
    (*pNodes).NodeCoo[i][2] *= scalez;
  }

  return;
}

void
neut_nodes_bbox (struct NODES Nodes, double *bbox)
{
  int i;

  for (i = 0; i < 3; i++)
  {
    bbox[2 * i] = 1e32;
    bbox[2 * i + 1] = -1e32;
  }

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    bbox[0] = ut_num_min (bbox[0], Nodes.NodeCoo[i][0]);
    bbox[1] = ut_num_max (bbox[1], Nodes.NodeCoo[i][0]);
    bbox[2] = ut_num_min (bbox[2], Nodes.NodeCoo[i][1]);
    bbox[3] = ut_num_max (bbox[3], Nodes.NodeCoo[i][1]);
    bbox[4] = ut_num_min (bbox[4], Nodes.NodeCoo[i][2]);
    bbox[5] = ut_num_max (bbox[5], Nodes.NodeCoo[i][2]);
  }

  return;
}

void
neut_nodes_init_bbox (struct NODES* pNodes)
{
  /* assuming a cubic box aligned with the X, Y and Z axes */
  int i, j;

  if ((*pNodes).BBox == NULL)
    (*pNodes).BBox = ut_alloc_2d (3, 2);

  (*pNodes).BBox[0][0] = (*pNodes).BBox[1][0] = (*pNodes).BBox[2][0] = DBL_MAX;
  (*pNodes).BBox[0][1] = (*pNodes).BBox[1][1] = (*pNodes).BBox[2][1] = -DBL_MAX;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    for (j = 0; j < 3; j++)
    {
      (*pNodes).BBox[j][0] = ut_num_min ((*pNodes).BBox[j][0], (*pNodes).NodeCoo[i][j]);
      (*pNodes).BBox[j][1] = ut_num_max ((*pNodes).BBox[j][1], (*pNodes).NodeCoo[i][j]);
    }

  return;
}


void
neut_nodes_set_zero (struct NODES* pNodes)
{
  (*pNodes).NodeQty = 0;
  (*pNodes).NodeCoo = NULL;
  (*pNodes).NodeCl = NULL;
  (*pNodes).BBox = NULL;

  return;
}

void
neut_nodes_free (struct NODES *pNodes)
{
  if (pNodes != NULL)
  {
    ut_free_2d ((*pNodes).NodeCoo, (*pNodes).NodeQty + 1);
    ut_free_2d ((*pNodes).BBox, 3);
    ut_free_1d ((*pNodes).NodeCl);
  }
  neut_nodes_set_zero (pNodes);

  return;
}

void
neut_nodes_addnode (struct NODES* pNodes, double* NodeCoo, double NodeCl)
{
  (*pNodes).NodeQty++;

  (*pNodes).NodeCoo = ut_realloc_2d_addline (
      (*pNodes).NodeCoo, (*pNodes).NodeQty + 1, 3);
  (*pNodes).NodeCoo[0] = NULL; /* mandatory */

  ut_array_1d_memcpy ((*pNodes).NodeCoo[(*pNodes).NodeQty], 3, NodeCoo);

  (*pNodes).NodeCl = ut_realloc_1d ((*pNodes).NodeCl, (*pNodes).NodeQty + 1);
  (*pNodes).NodeCl[(*pNodes).NodeQty] = NodeCl;

  return;
}

void
neut_nodes_memcpy (struct NODES Nodes, struct NODES* pNodes2)
{
  neut_nodes_free (pNodes2);

  (*pNodes2).NodeQty = Nodes.NodeQty;
  (*pNodes2).NodeCoo = ut_alloc_2d (Nodes.NodeQty + 1, 3);

  ut_array_2d_memcpy ((*pNodes2).NodeCoo + 1, Nodes.NodeQty, 3, Nodes.NodeCoo + 1);

  if (Nodes.NodeCl != NULL)
  {
    (*pNodes2).NodeCl = ut_alloc_1d (Nodes.NodeQty + 1);
    ut_array_1d_memcpy ((*pNodes2).NodeCl + 1, Nodes.NodeQty, Nodes.NodeCl + 1);
  }

  if (Nodes.BBox != NULL)
  {
    (*pNodes2).BBox = ut_alloc_2d (3, 2);
    ut_array_2d_memcpy ((*pNodes2).BBox, 3, 2, Nodes.BBox);
  }

  return;
}

void
neut_nodes_proj_alongontomesh (struct NODES *pN, double* n, struct NODES N, struct MESH M, int elset)
{
  int i, j, elt, status;
  double* coo = ut_alloc_1d (3);
  double* eq = ut_alloc_1d (4);

  // FILE* file;
  // struct PART PGarbage;
  // sprintf (message, "face-%d-bef.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");
  
  for (i = 1; i <= (*pN).NodeQty; i++)
  {
    status = 0;
    
    for (j = 1; j <= M.Elsets[elset][0]; j++)
    {
      elt = M.Elsets[elset][j];
      ut_array_1d_memcpy (coo, 3, (*pN).NodeCoo[i]);
      neut_mesh_elt_eq (M, N, elt, eq);

      ut_space_projpoint_alongonto (coo, n, eq);

      if (ut_space_triangle_point_in (N.NodeCoo[M.EltNodes[elt][0]],
	    N.NodeCoo[M.EltNodes[elt][1]], N.NodeCoo[M.EltNodes[elt][2]], coo) == 1)
      {
	ut_array_1d_memcpy ((*pN).NodeCoo[i], 3, coo);
	status = 1;
	break;
      }
    }

    if (status == 0)
    {
      printf ("\nnode not caught during backward projection\n");
      ut_error_reportbug ();
    }
  }

  ut_free_1d (coo);
  ut_free_1d (eq);
  
  // sprintf (message, "face-%d-aft.msh", i);
  // file = ut_file_open (message, "w");
  // WriteMeshGmsh (file, N, Garbage, M, Garbage, NULL, NULL, NULL, PGarbage);
  // ut_file_close (file, message, "w");

  return;
}

int
neut_nodes_point_closestnode (struct NODES Nodes, double* coo, int* pnode)
{
  int i;
  double* dist = ut_alloc_1d (Nodes.NodeQty + 1);

  for (i = 1; i <= Nodes.NodeQty; i++)
    dist[i] = ut_space_dist (Nodes.NodeCoo[i], coo);

  (*pnode) = ut_array_1d_min_index (dist + 1, Nodes.NodeQty);

  ut_free_1d (dist);

  return 0;
}

int
neut_nodes_rmorphans (struct NODES* pNodes, struct MESH* pMesh, struct NSET* pNSet2D)
{
  int i, j, nodeqty;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  if ((*pMesh).EltQty == 0)
  {
    neut_nodes_free (pNodes);
    neut_nodes_set_zero (pNodes);
    return 1;
  }

  if ((*pMesh).NodeElts == NULL)
    neut_mesh_init_nodeelts (pMesh, (*pNodes).NodeQty);

  int* old_new = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  int* new_old = ut_alloc_1d_int ((*pNodes).NodeQty + 1);

  nodeqty = 0;
  for (i = 1; i <= (*pNodes).NodeQty; i++)
    if ((*pMesh).NodeElts[i][0] != 0)
    {
      old_new[i] = ++nodeqty;
      new_old[old_new[i]] = i;
    }

  for (i = 1; i <= nodeqty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[i], 3, (*pNodes).NodeCoo[new_old[i]]);

  if ((*pNodes).NodeCl != NULL)
    for (i = 1; i <= nodeqty; i++)
      (*pNodes).NodeCl[i] = (*pNodes).NodeCl[new_old[i]];
  
  (*pNodes).NodeCoo = ut_realloc_2d_delline
                        ((*pNodes).NodeCoo, (*pNodes).NodeQty + 1, nodeqty + 1);
  (*pNodes).NodeCl = ut_realloc_1d ((*pNodes).NodeCl, nodeqty + 1);

  (*pNodes).NodeQty = nodeqty;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).EltNodes[i][j] = old_new[(*pMesh).EltNodes[i][j]];

  if (pNSet2D != NULL)
    for (i = 1; i <= (*pNSet2D).qty; i++)
    {
      for (j = 0; j < (*pNSet2D).nodeqty[i]; j++)
	(*pNSet2D).nodes[i][j] = old_new[(*pNSet2D).nodes[i][j]];

      (*pNSet2D).nodeqty[i]
	-= ut_array_1d_int_deletencompress ((*pNSet2D).nodes[i],
	        (*pNSet2D).nodeqty[i], 0, (*pNSet2D).nodeqty[i]);
    }

  ut_free_1d_int (old_new);
  ut_free_1d_int (new_old);

  return 0;
}

void
neut_nodes_switch_pair (struct NODES *pNodes, int n1, int n2)
{
  ut_array_2d_switchlines ((*pNodes).NodeCoo, 3, n1, n2);

  return;
}

void
neut_nodes_switch (struct NODES *pNodes, int* node_nbs)
{
  int i;
  double** coo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy (coo[i], 3, (*pNodes).NodeCoo[i]);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    ut_array_1d_memcpy ((*pNodes).NodeCoo[node_nbs[i]], 3, coo[i]);

  ut_free_2d (coo, (*pNodes).NodeQty);

  return;
}

double
neut_nodes_dist_pair (struct NODES Nodes, int n1, int n2)
{
  return ut_space_dist (Nodes.NodeCoo[n1], Nodes.NodeCoo[n2]);
}
