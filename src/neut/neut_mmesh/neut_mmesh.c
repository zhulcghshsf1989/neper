/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mmesh.h"

extern void
neut_mmesh_elset_fscanf (FILE* file, struct ELSET *pElset, int MaxElsetQty)
{
  int skip, i, test, elset;
  char *String = ut_alloc_1d_char (10000);
  char *trash = ut_alloc_1d_char (1000);
  int *EltQty = ut_alloc_1d_int (MaxElsetQty + 1);
  int tmp;

  (*pElset).Qty = 0;

  /* Counting the number of elsets */
  test = 0;
  tmp = 0;
  skip = -1;
  while (test != -1 && strcmp (String, "***return") != 0)
  {
    if (strcmp (String, "**elset") == 0)
    {
      if (skip == -1)
	skip = tmp - 1;
      (*pElset).Qty++;
      EltQty[(*pElset).Qty - 1] = tmp - 1;
      test = fscanf (file, "%s", trash);
      tmp = 0;
    }

    test = fscanf (file, "%s", String);
    tmp++;
  }
  EltQty[0] = 0;
  EltQty[(*pElset).Qty] = tmp - 1;

  fseek (file, 0, 0);

  /* Reading elsets */
  (*pElset).Elset = ut_alloc_1d_pint ((*pElset).Qty + 1);
  for (i = 1; i <= (*pElset).Qty; i++)
  {
    (*pElset).Names = ut_alloc_2d_char ((*pElset).Qty + 1, 100);
    (*pElset).Id = ut_alloc_1d_int ((*pElset).Qty + 1);
    (*pElset).Elset[i] = ut_alloc_1d_int (EltQty[i] + 1);
    (*pElset).Elset[i][0] = EltQty[i];
  }

  /* Reaching the first elset */
  ut_file_skip (file, skip);

  /* Reading elsets */
  for (elset = 1; elset <= (*pElset).Qty; elset++)
  {
    ut_file_skip (file, 1);
    if (fscanf (file, "%s", (*pElset).Names[elset]) != 1)
      abort ();
    ut_array_1d_int_fscanf (file, (*pElset).Elset[elset] + 1,
				  (*pElset).Elset[elset][0]);
  }

  /* Extracting name Body and id */
  tmp = 0;
  (*pElset).Body = ut_alloc_1d_char (1000);
  for (i = 0; i < (int) strlen ((*pElset).Names[1]); i++)
    if (((*pElset).Names[1][tmp] >= 'a'
	 && (*pElset).Names[1][tmp] <= 'z')
	|| ((*pElset).Names[1][tmp] >= 'A' && (*pElset).Names[1][tmp] <= 'Z'))
    {
      (*pElset).Body[tmp] = (*pElset).Names[1][tmp];
      tmp++;
    }
    else
    {
      (*pElset).Body[++tmp] = '\0';
      break;
    }

  for (elset = 1; elset <= (*pElset).Qty; elset++)
  {
    sscanf ((*pElset).Names[elset] + strlen ((*pElset).Body),
	    "%d", &(*pElset).Id[elset]);
  }

  ut_free_1d_int (EltQty);
  ut_free_1d_char (String);
  ut_free_1d_char (trash);

  return;
}

void
neut_mmesh_2d_fprintf (struct NODES Nodes, struct MESH Mesh, char *filename)
{
  int i, j;
  FILE *file = ut_file_open (filename, "W");

  fprintf (file, "***geometry\n");

  /* Writing nodes */
  fprintf (file, "**node\n");
  fprintf (file, "%d 2\n", Nodes.NodeQty);

  for (i = 1; i <= Nodes.NodeQty; i++)
    fprintf (file, "%d %.8f %.8f\n", i,
	     Nodes.NodeCoo[i][0], Nodes.NodeCoo[i][1]);

  /* Writing elements */
  fprintf (file, "**element\n");
  fprintf (file, "%d\n", Mesh.EltQty);

  if (Mesh.EltOrder == 1)	/* mesh Order = 1 */
  {
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%3d c2d4", i);
      for (j = 1; j <= 4; j++)
	fprintf (file, " %4d", Mesh.EltNodes[i][j]);
      fprintf (file, "\n");
    }
  }
  else				/* mesh Order = 2 */
  {
    for (i = 1; i <= Mesh.EltQty; i++)
    {
      fprintf (file, "%3d c2d8", i);
      fprintf (file, " %4d", Mesh.EltNodes[i][0]);
      fprintf (file, " %4d", Mesh.EltNodes[i][4]);
      fprintf (file, " %4d", Mesh.EltNodes[i][1]);
      fprintf (file, " %4d", Mesh.EltNodes[i][5]);
      fprintf (file, " %4d", Mesh.EltNodes[i][2]);
      fprintf (file, " %4d", Mesh.EltNodes[i][6]);
      fprintf (file, " %4d", Mesh.EltNodes[i][3]);
      fprintf (file, " %4d", Mesh.EltNodes[i][7]);
      fprintf (file, "\n");
    }
  }

  fprintf (file, "\n***return\n");

  ut_file_close (file, filename, "W");

  return;
}

void
BuildMMesh3D (int *size, int order,
	      struct NODES *pNodes, struct MESH *pMesh, int ***pFoDNodes)
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

  // SearchFoDNodes_cube (*pNodes, pFoDNodes);
  pFoDNodes = pFoDNodes;

  ut_free_1d_int (nqty);

  return;
}

void
BuildMMesh3DCentre (int sizeX, int sizeY, int sizeZ, int *pEltQty,
		    double ***pEltCoo)
{
  int x, y, z, eltid;

  (*pEltQty) = sizeX * sizeY * sizeZ;
  (*pEltCoo) = ut_alloc_2d ((*pEltQty) + 1, 3);

  eltid = 0;
  for (z = 1; z <= sizeZ; z++)
    for (y = 1; y <= sizeY; y++)
      for (x = 1; x <= sizeX; x++)
      {
	eltid++;
	(*pEltCoo)[eltid][0] = ((double) x - 0.5) / (double) sizeX;
	(*pEltCoo)[eltid][1] = ((double) y - 0.5) / (double) sizeY;
	(*pEltCoo)[eltid][2] = ((double) z - 0.5) / (double) sizeZ;
      }

  return;
}

void
SearchFoDNodes_cube (struct NODES Nodes, int ***pFoDNodes)
{
  int face, i, coo;
  int* tmp = ut_alloc_1d_int (Nodes.NodeQty + 1);
  double* domain = ut_alloc_1d (6);
  double eps = 1e-12;

  neut_nodes_domain (Nodes, domain);

  for (i = 0; i < 3; i++)
  {
    domain[2 * i] += eps;
    domain[2 * i + 1] -= eps;
  }

  (*pFoDNodes) = ut_alloc_1d_pint (7);

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
	if (Nodes.NodeCoo[i][coo] > domain [face - 1])
	  tmp[++tmp[0]] = i;
    }

    (*pFoDNodes)[face] = ut_alloc_1d_int (tmp[0] + 1);
    ut_array_1d_int_memcpy ((*pFoDNodes)[face], tmp[0] + 1, tmp);
  }
  
  return;
}

int
EltCoo2Id (int X, int Y, int Z, int XEQty, int YEQty)
{
  return (Z - 1) * YEQty * XEQty + (Y - 1) * XEQty + X;
}
