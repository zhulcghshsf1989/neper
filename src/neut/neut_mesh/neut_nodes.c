/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_nodes.h"

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
neut_nodes_shift (struct NODES *pNodes, double shiftx, double shifty,
		  double shiftz)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    (*pNodes).NodeCoo[i][0] += shiftx;
    (*pNodes).NodeCoo[i][1] += shifty;
    (*pNodes).NodeCoo[i][2] += shiftz;
  }

  return;
}

void
neut_nodes_domain (struct NODES Nodes, double *domain)
{
  int i;

  for (i = 0; i < 3; i++)
  {
    domain[2 * i] = 1e32;
    domain[2 * i + 1] = -1e32;
  }

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    domain[0] = ut_num_min (domain[0], Nodes.NodeCoo[i][0]);
    domain[1] = ut_num_max (domain[1], Nodes.NodeCoo[i][0]);
    domain[2] = ut_num_min (domain[2], Nodes.NodeCoo[i][1]);
    domain[3] = ut_num_max (domain[3], Nodes.NodeCoo[i][1]);
    domain[4] = ut_num_min (domain[4], Nodes.NodeCoo[i][2]);
    domain[5] = ut_num_max (domain[5], Nodes.NodeCoo[i][2]);
  }

  return;
}

void
neut_nodes_init_boundingbox (struct NODES* pNodes)
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
neut_nodes_fodnodes (struct NODES Nodes, int **NSets)
{
  int face, i, coo;
  int *tmp = ut_alloc_1d_int (Nodes.NodeQty + 1);
  double *domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_domain (Nodes, domain);

  for (i = 0; i < 3; i++)
  {
    domain[2 * i] += eps;
    domain[2 * i + 1] -= eps;
  }

  for (face = 1; face <= 6; face++)
  {
    if (face == 1 || face == 2)
      coo = 0;
    else if (face == 3 || face == 4)
      coo = 1;
    else
      coo = 2;

    tmp[0] = 0;
    if (face % 2 == 1)
    {
      for (i = 1; i <= Nodes.NodeQty; i++)
	if (Nodes.NodeCoo[i][coo] < domain[face - 1])
	  tmp[++tmp[0]] = i;
    }
    else
    {
      for (i = 1; i <= Nodes.NodeQty; i++)
	if (Nodes.NodeCoo[i][coo] > domain[face - 1])
	  tmp[++tmp[0]] = i;
    }

    NSets[face] = ut_alloc_1d_int (tmp[0] + 1);
    ut_array_1d_int_memcpy (NSets[face], tmp[0] + 1, tmp);
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
neut_nodes_fprintf_scotch (FILE * out, struct NODES Nodes)
{
  int i, j;

  fprintf (out, "3 %d\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (out, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (out, "  %.12f", Nodes.NodeCoo[i][j]);
    fprintf (out, "\n");
  }

  return;
}

void
neut_nodes_fprintf_stellar (FILE * out, struct NODES Nodes)
{
  int i, j;
  int bound;

  fprintf (out, "%d 3 0 1\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (out, "%d", i);
    bound = 0;
    for (j = 0; j < 3; j++)
      if (Nodes.NodeCoo[i][j] < 1e-12 || Nodes.NodeCoo[i][j] > 1-1e-12)
      {
	bound = 1;
	break;
      }

    for (j = 0; j < 3; j++)
      fprintf (out, "  %.12f", Nodes.NodeCoo[i][j]);
    fprintf (out, " %d\n", bound);
  }

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
neut_nodes_nodes (struct NODES Old, struct NODES* pNew)
{
  neut_nodes_free (pNew);

  (*pNew).NodeQty = Old.NodeQty;
  (*pNew).NodeCoo = ut_alloc_2d (Old.NodeQty + 1, 3);

  ut_array_2d_memcpy ((*pNew).NodeCoo + 1, Old.NodeQty, 3, Old.NodeCoo + 1);

  if (Old.NodeCl != NULL)
  {
    (*pNew).NodeCl = ut_alloc_1d (Old.NodeQty + 1);
    ut_array_1d_memcpy ((*pNew).NodeCl + 1, Old.NodeQty, Old.NodeCl + 1);
  }

  if (Old.BBox != NULL)
  {
    (*pNew).BBox = ut_alloc_2d (3, 2);
    ut_array_2d_memcpy ((*pNew).BBox, 3, 2, Old.BBox);
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
      neut_mesh_elteq (M, N, elt, eq);

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
neut_point_proj_alongontomesh (double* Coo, double* n, struct NODES N, struct MESH M, int elset)
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
    neut_mesh_elteq (M, N, elt, eq);
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
neut_nodes_fprintf_gmsh (FILE* file, struct NODES Nodes)
{
  int i, j;

  fprintf (file, "$Nodes\n");
  fprintf (file, "%d\n", Nodes.NodeQty);
  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (file, " %.12f",
	  (fabs (Nodes.NodeCoo[i][j]) < 1e-12) ? 0 : Nodes.NodeCoo[i][j]);
    fprintf (file, "\n");
  }
  fprintf (file, "$EndNodes\n");

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
neut_nodes_eltdim (struct MESH Mesh0D,
                   struct MESH Mesh1D, struct MESH Mesh2D,
		   struct MESH Mesh3D, int node)
{
  int j, dim = -1;

  int eltnodeqty0d, eltnodeqty1d, eltnodeqty2d;

  eltnodeqty0d = neut_elt_nodeqty (Mesh0D.EltType, Mesh0D.Dimension, Mesh0D.EltOrder);
  eltnodeqty1d = neut_elt_nodeqty (Mesh1D.EltType, Mesh1D.Dimension, Mesh1D.EltOrder);
  eltnodeqty2d = neut_elt_nodeqty (Mesh2D.EltType, Mesh2D.Dimension, Mesh2D.EltOrder);

  for (j = 1; dim == -1 && j <= Mesh0D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh0D.EltNodes[j], eltnodeqty0d, node) != -1)
      dim = 0;
      
  for (j = 1; dim == -1 && j <= Mesh1D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh1D.EltNodes[j], eltnodeqty1d, node) != -1)
      dim = 1;

  for (j = 1; dim == -1 && j <= Mesh2D.EltQty; j++)
    if (ut_array_1d_int_eltpos (Mesh2D.EltNodes[j], eltnodeqty2d, node) != -1)
      dim = 2;

  if (dim == -1)
  {
    Mesh3D = Mesh3D;
    dim = 3;
  }

  return dim;
}

int
neut_nodes_rmorphans (struct NODES* pNodes, struct MESH* pMesh, int*** pFoDNodes)
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

  if (pFoDNodes != NULL)
    for (i = 1; i <= 6; i++)
    {
      for (j = 1; j <= (*pFoDNodes)[i][0]; j++)
	(*pFoDNodes)[i][j] = old_new[(*pFoDNodes)[i][j]];

      (*pFoDNodes)[i][0] -= ut_array_1d_int_deletencompress
   	        ((*pFoDNodes)[i] + 1, (*pFoDNodes)[i][0], 0, (*pFoDNodes)[i][0]);
    }

  ut_free_1d_int (old_new);
  ut_free_1d_int (new_old);

  return 0;
}
