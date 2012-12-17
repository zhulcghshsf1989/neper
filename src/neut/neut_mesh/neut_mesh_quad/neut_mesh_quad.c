/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_quad.h"

void
neut_mesh_quad (int *size, int order, struct NODES *pNodes, struct MESH
    *pMesh, struct NSET* pNSet2D)
{
  int i, j, k, elt, x, y, z, pos1, pos2, nid, eid, nodesperelt;
  double nx, ny, nz;
  int *nqty = ut_alloc_1d_int (3);

  /* General ------------------------------------------------------- */

  (*pMesh).msize = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pMesh).msize, 3, size);

  (*pMesh).EltOrder = order;
  (*pMesh).Dimension = 3;
  (*pMesh).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh).EltType, "quad");

  nodesperelt = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
      (*pMesh).EltOrder);
  
  (*pMesh).EltQty   = (*pMesh).msize[0] * (*pMesh).msize[1] * (*pMesh).msize[2];

  /* Calculating element coordinates ------------------------------- */

  // Filling EltCoo
  (*pMesh).EltCoo   = ut_alloc_2d ((*pMesh).EltQty + 1, 3);

  elt = 0;
  for (k = 1; k <= (*pMesh).msize[2]; k++)
    for (j = 1; j <= (*pMesh).msize[1]; j++)
      for (i = 1; i <= (*pMesh).msize[0]; i++)
      {
	elt++;
	((*pMesh).EltCoo[elt][0] = ((double) i - 0.5) /
	                           (double) (*pMesh).msize[0]);
	((*pMesh).EltCoo[elt][1] = ((double) j - 0.5) /
	                           (double) (*pMesh).msize[1]);
	((*pMesh).EltCoo[elt][2] = ((double) k - 0.5) /
	                           (double) (*pMesh).msize[2]);
      }

  /* Calculating NodeCoo and NodeQty ------------------------------- */

  for (i = 0; i < 3; i++)
  {
    pos1 = ut_num_rotpos (0, 2, i, 1);
    pos2 = ut_num_rotpos (0, 2, i, 2);
    nqty[i] = (size[pos1] + 1) * (size[pos2] + 1);
    if (order == 2)
      nqty[i] += size[pos1] * (size[pos2] + 1)
	+ size[pos2] * (size[pos1] + 1);
  }

  int ***O1NMap = NULL;
  int ***O2XNMap = NULL;
  int ***O2YNMap = NULL;
  int ***O2ZNMap = NULL;
  O1NMap  = ut_alloc_3d_int (size[0] + 2, size[1] + 2, size[2] + 2);
  O2XNMap = ut_alloc_3d_int (size[0] + 1, size[1] + 2, size[2] + 2);
  O2YNMap = ut_alloc_3d_int (size[0] + 2, size[1] + 1, size[2] + 2);
  O2ZNMap = ut_alloc_3d_int (size[0] + 2, size[1] + 2, size[2] + 1);

  (*pNodes).NodeQty = (size[0] + 1) * (size[1] + 1) * (size[2] + 1);
  if (order == 2)
  {
    (*pNodes).NodeQty += size[0] * (size[1] + 1) * (size[2] + 1);
    (*pNodes).NodeQty += (size[0] + 1) * size[1] * (size[2] + 1);
    (*pNodes).NodeQty += (size[0] + 1) * (size[1] + 1) * size[2];
  }

  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  /* Geometry --- */
  nid = 0;

  /* 1st-Order nodes */
  for (z = 1; z <= size[2] + 1; z++)
  {
    nz = ((double) (z - 1)) / ((double) (size[2]));

    for (y = 1; y <= size[1] + 1; y++)
    {
      ny = ((double) (y - 1)) / ((double) (size[1]));

      for (x = 1; x <= size[0] + 1; x++)
      {
	nid++;
	nx = ((double) (x - 1)) / ((double) (size[0]));

	/* Map */
	O1NMap[x][y][z] = nid;

	/* NodeCoo */
	(*pNodes).NodeCoo[nid][0] = nx;
	(*pNodes).NodeCoo[nid][1] = ny;
	(*pNodes).NodeCoo[nid][2] = nz;
      }
    }
  }

  if ((*pMesh).EltOrder == 2)	/* 2nd-order nodes */
  {
    /* 2nd-Order nodes: X */
    for (z = 1; z <= size[2] + 1; z++)
    {
      nz = ((double) (z - 1)) / ((double) (size[2]));

      for (y = 1; y <= size[1] + 1; y++)
      {
	ny = ((double) (y - 1)) / ((double) (size[1]));

	for (x = 1; x <= size[0]; x++)
	{
	  nid++;
	  nx = ((double) (x) - 0.5) / ((double) (size[0]));

	  /* Map */
	  O2XNMap[x][y][z] = nid;

	  /* NodeCoo */
	  (*pNodes).NodeCoo[nid][0] = nx;
	  (*pNodes).NodeCoo[nid][1] = ny;
	  (*pNodes).NodeCoo[nid][2] = nz;
	}
      }
    }

    /* 2nd-Order nodes: Y */
    for (z = 1; z <= size[2] + 1; z++)
    {
      nz = ((double) (z - 1)) / ((double) (size[2]));

      for (y = 1; y <= size[1]; y++)
      {
	ny = ((double) (y) - 0.5) / ((double) (size[1]));

	for (x = 1; x <= size[0] + 1; x++)
	{
	  nid++;
	  nx = ((double) (x - 1)) / ((double) (size[0]));

	  /* Map */
	  O2YNMap[x][y][z] = nid;

	  /* NodeCoo */
	  (*pNodes).NodeCoo[nid][0] = nx;
	  (*pNodes).NodeCoo[nid][1] = ny;
	  (*pNodes).NodeCoo[nid][2] = nz;
	}
      }
    }

    /* 2nd-Order nodes: Z */
    for (z = 1; z <= size[2]; z++)
    {
      nz = ((double) (z) - 0.5) / ((double) (size[2]));

      for (y = 1; y <= size[1] + 1; y++)
      {
	ny = ((double) (y - 1)) / ((double) (size[1]));

	for (x = 1; x <= size[0] + 1; x++)
	{
	  nid++;
	  nx = ((double) (x - 1)) / ((double) (size[0]));

	  /* Map */
	  O2ZNMap[x][y][z] = nid;

	  /* NodeCoo */
	  (*pNodes).NodeCoo[nid][0] = nx;
	  (*pNodes).NodeCoo[nid][1] = ny;
	  (*pNodes).NodeCoo[nid][2] = nz;
	}
      }
    }
  }

  /* Connectivity */

  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, nodesperelt);

  eid = 0;

  /* Recording the 1st-order nodes */
  if (order == 1)
  {
    for (z = 1; z <= size[2]; z++)
      for (y = 1; y <= size[1]; y++)
	for (x = 1; x <= size[0]; x++)
	{
	  eid++;

	  (*pMesh).EltNodes[eid][0] = O1NMap[x][y][z];
	  (*pMesh).EltNodes[eid][1] = O1NMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][2] = O1NMap[x + 1][y + 1][z];
	  (*pMesh).EltNodes[eid][3] = O1NMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][4] = O1NMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][5] = O1NMap[x + 1][y][z + 1];
	  (*pMesh).EltNodes[eid][6] = O1NMap[x + 1][y + 1][z + 1];
	  (*pMesh).EltNodes[eid][7] = O1NMap[x][y + 1][z + 1];
	}
  }
  else
  {
    /* Recording the 1st-order nodes */
    for (z = 1; z <= size[2]; z++)
      for (y = 1; y <= size[1]; y++)
	for (x = 1; x <= size[0]; x++)
	{
	  eid++;
	  
	  (*pMesh).EltNodes[eid][0]  = O1NMap[x][y][z];
	  (*pMesh).EltNodes[eid][1]  = O1NMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][2]  = O1NMap[x + 1][y + 1][z];
	  (*pMesh).EltNodes[eid][3]  = O1NMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][4]  = O1NMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][5]  = O1NMap[x + 1][y][z + 1];
	  (*pMesh).EltNodes[eid][6]  = O1NMap[x + 1][y + 1][z + 1];
	  (*pMesh).EltNodes[eid][7]  = O1NMap[x][y + 1][z + 1];
	  
	  (*pMesh).EltNodes[eid][8]  = O2XNMap[x][y][z];
	  (*pMesh).EltNodes[eid][9]  = O2YNMap[x][y][z];
	  (*pMesh).EltNodes[eid][10] = O2ZNMap[x][y][z];
	  (*pMesh).EltNodes[eid][11] = O2YNMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][12] = O2ZNMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][13] = O2XNMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][14] = O2ZNMap[x + 1][y + 1][z];
	  (*pMesh).EltNodes[eid][15] = O2ZNMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][16] = O2XNMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][17] = O2YNMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][18] = O2YNMap[x + 1][y][z + 1];
	  (*pMesh).EltNodes[eid][19] = O2XNMap[x][y + 1][z + 1];
	}
  }

  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

  ut_free_3d_int (O1NMap, size[0] + 2, size[1] + 2);
  ut_free_3d_int (O2XNMap, size[0] + 1, size[1] + 2);
  ut_free_3d_int (O2YNMap, size[0] + 2, size[1] + 1);
  ut_free_3d_int (O2ZNMap, size[0] + 2, size[1] + 2);

  // Searching nsets

  int face, coo;
  int* tmp = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  double* domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_bbox ((*pNodes), domain);

  for (i = 0; i < 3; i++)
  {
    domain[2 * i] += eps;
    domain[2 * i + 1] -= eps;
  }

  (*pNSet2D).qty     = 6;
  (*pNSet2D).names   = ut_alloc_2d_char (7, 10);
  (*pNSet2D).nodeqty = ut_alloc_1d_int ((*pNSet2D).qty + 1);
  (*pNSet2D).nodes   = ut_alloc_1d_pint ((*pNSet2D).qty + 1);

  strcpy ((*pNSet2D).names[1], "x0");
  strcpy ((*pNSet2D).names[2], "x1");
  strcpy ((*pNSet2D).names[3], "y0");
  strcpy ((*pNSet2D).names[4], "y1");
  strcpy ((*pNSet2D).names[5], "z0");
  strcpy ((*pNSet2D).names[6], "z1");

  for (face = 1; face <= 6; face++)
  {
    coo = (face - 1) / 2;

    tmp[0] = 0;
    if (face % 2 == 1)
    {
      for (i = 1; i <= (*pNodes).NodeQty; i++)
	if ((*pNodes).NodeCoo[i][coo] < domain[face - 1])
	  tmp[++tmp[0]] = i;
    }
    else
    {
      for (i = 1; i <= (*pNodes).NodeQty; i++)
	if ((*pNodes).NodeCoo[i][coo] > domain [face - 1])
	  tmp[++tmp[0]] = i;
    }

    (*pNSet2D).nodeqty[face] = tmp[0];
    (*pNSet2D).nodes[face] = ut_alloc_1d_int ((*pNSet2D).nodeqty[face]);
    ut_array_1d_int_memcpy ((*pNSet2D).nodes[face], tmp[0], tmp + 1);
  }

  ut_free_1d_int (nqty);
  
  return;
}
