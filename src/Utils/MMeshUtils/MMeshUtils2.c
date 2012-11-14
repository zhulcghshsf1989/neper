/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "MMeshUtils.h"

/* BuildMesh builds the mesh: geometry & connectivity */
void
BuildMMesh2D (int XEQty, int YEQty, int order,
	      struct NODES *pNodes, struct MESH *pMesh)
{
  int x, y;			/* mute variables                            */
  double nx, ny;		/* tmp variables for "node x" and "node y"   */
  int eid, nid, nodesperelt;	/* tmp variables for "elt id" and "node id"  */

  /* note: the arrays begin from [1][1]. */
  int **O1NMap = twoDIntDAlloc (XEQty + 2, YEQty + 2);
  int **O2XNMap = twoDIntDAlloc (XEQty + 1, YEQty + 2);
  int **O2YNMap = twoDIntDAlloc (XEQty + 2, YEQty + 1);

  (*pMesh).EltOrder = order;

  if ((*pMesh).EltOrder == 1)
    nodesperelt = 8;
  else if ((*pMesh).EltOrder == 2)
    nodesperelt = 20;
  else
    abort ();

  (*pMesh).EltQty = XEQty * YEQty;

  (*pNodes).NodeQty = (XEQty + 1) * (YEQty + 1);
  if (order == 2)
  {
    (*pNodes).NodeQty += XEQty * (YEQty + 1);
    (*pNodes).NodeQty += (XEQty + 1) * YEQty;
  }

  (*pNodes).NodeCoo = twoDDoubleDAlloc ((*pNodes).NodeQty + 1, 3);
  (*pMesh).EltNodes = twoDIntDAlloc ((*pMesh).EltQty + 1, nodesperelt);

  /* Geometry --- */
  nid = 0;

  /* 1st-Order nodes */
  for (y = 1; y <= YEQty + 1; y++)
  {
    ny = ((double) (y - 1)) / ((double) (YEQty));

    for (x = 1; x <= XEQty + 1; x++)
    {
      nid++;
      nx = ((double) (x - 1)) / ((double) (XEQty));

      /* Map */
      O1NMap[x][y] = nid;

      /* NodeCoo */
      (*pNodes).NodeCoo[nid][0] = nx;
      (*pNodes).NodeCoo[nid][1] = ny;
    }
  }

  if ((*pMesh).EltOrder == 2)	/* 2nd-Order nodes */
  {
    /* 2nd-Order nodes: X */
    for (y = 1; y <= YEQty + 1; y++)
    {
      ny = ((double) (y - 1)) / ((double) (YEQty));

      for (x = 1; x <= XEQty; x++)
      {
	nid++;
	nx = ((double) (x) - 0.5) / ((double) (XEQty));

	/* Map */
	O2XNMap[x][y] = nid;

	/* NodeCoo */
	(*pNodes).NodeCoo[nid][0] = nx;
	(*pNodes).NodeCoo[nid][1] = ny;
      }
    }

    /* 2nd-Order nodes: Y */
    for (y = 1; y <= YEQty; y++)
    {
      ny = ((double) (y) - 0.5) / ((double) (YEQty));

      for (x = 1; x <= XEQty + 1; x++)
      {
	nid++;
	nx = ((double) (x - 1)) / ((double) (XEQty));

	/* Map */
	O2YNMap[x][y] = nid;

	/* NodeCoo */
	(*pNodes).NodeCoo[nid][0] = nx;
	(*pNodes).NodeCoo[nid][1] = ny;
      }
    }
  }


  /* Connectivity --- */

  eid = 0;

  /* Recording the 1st-Order nodes */
  for (y = 1; y <= YEQty; y++)
    for (x = 1; x <= XEQty; x++)
    {
      eid++;

      (*pMesh).EltNodes[eid][0] = O1NMap[x][y];
      (*pMesh).EltNodes[eid][1] = O1NMap[x + 1][y];
      (*pMesh).EltNodes[eid][2] = O1NMap[x + 1][y + 1];
      (*pMesh).EltNodes[eid][3] = O1NMap[x][y + 1];
    }

  if ((*pMesh).EltOrder == 2)	/* if Order==2, */
  {				/* recording the 2nd-Order nodes */
    eid = 0;

    for (y = 1; y <= YEQty; y++)
      for (x = 1; x <= XEQty; x++)
      {
	eid++;

	(*pMesh).EltNodes[eid][4] = O2XNMap[x][y];
	(*pMesh).EltNodes[eid][5] = O2YNMap[x + 1][y];
	(*pMesh).EltNodes[eid][6] = O2XNMap[x][y + 1];
	(*pMesh).EltNodes[eid][7] = O2YNMap[x][y];
      }
  }

  /* Memory freeing */

  twoDIntFree (O1NMap, XEQty + 2);
  twoDIntFree (O2XNMap, XEQty + 1);
  twoDIntFree (O2YNMap, XEQty + 2);

  return;
}

/* BuildMesh3D builds a 3D mesh: nodes, elts, nsets.  */
void
BuildMMesh3D (int *size, int order,
	      struct NODES *pNodes, struct MESH *pMesh, int ***pFoDNodes)
{
  int i, j, k, elt;
  int x, y, z;
  double nx, ny, nz;
  int nid, eid, nodesperelt;
  int ***O1NMap;
  int ***O2XNMap;
  int ***O2YNMap;
  int ***O2ZNMap;
  int pos1, pos2;
  int *nqty = ut_alloc_1d_int (3);

  /* Calculating element coordinates ------------------------------- */

  (*pMesh).msize = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pMesh).msize, 3, size);
  
  (*pMesh).EltQty   = (*pMesh).msize[0] * (*pMesh).msize[1] * (*pMesh).msize[2];
  (*pMesh).EltCoo   = ut_alloc_2d ((*pMesh).EltQty + 1, 3);
  (*pMesh).EltElset = ut_alloc_1d_int ((*pMesh).EltQty + 1);

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

  /* */

  for (i = 0; i < 3; i++)
  {
    pos1 = oneDRotPos (0, 2, i, 1);
    pos2 = oneDRotPos (0, 2, i, 2);
    nqty[i] = (size[pos1] + 1) * (size[pos2] + 1);
    if (order == 2)
      nqty[i] += size[pos1] * (size[pos2] + 1)
	+ size[pos2] * (size[pos1] + 1);
  }

  (*pMesh).EltOrder = order;
  (*pMesh).Dimension = 3;
  (*pMesh).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh).EltType, "quad");

  nodesperelt = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension,
      (*pMesh).EltOrder);

  /* Memory allocations --- */

  /* note: the arrays begin from [1][1]. */
  O1NMap  = threeDIntDAlloc (size[0] + 2, size[1] + 2, size[2] + 2);
  O2XNMap = threeDIntDAlloc (size[0] + 1, size[1] + 2, size[2] + 2);
  O2YNMap = threeDIntDAlloc (size[0] + 2, size[1] + 1, size[2] + 2);
  O2ZNMap = threeDIntDAlloc (size[0] + 2, size[1] + 2, size[2] + 1);

  (*pMesh).EltQty = size[0] * size[1] * size[2];

  (*pNodes).NodeQty = (size[0] + 1) * (size[1] + 1) * (size[2] + 1);
  if (order == 2)
  {
    (*pNodes).NodeQty += size[0] * (size[1] + 1) * (size[2] + 1);
    (*pNodes).NodeQty += (size[0] + 1) * size[1] * (size[2] + 1);
    (*pNodes).NodeQty += (size[0] + 1) * (size[1] + 1) * size[2];
  }

  (*pNodes).NodeCoo = twoDDoubleDAlloc ((*pNodes).NodeQty + 1, 3);
  (*pMesh).EltNodes = twoDIntDAlloc ((*pMesh).EltQty + 1, nodesperelt);

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

  eid = 0;

  /* Recording the 1st-order nodes */
  if (order == 1)
  {
    eid = 0;

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

	  /*
	  (*pMesh).EltNodes[eid][0] = O1NMap[x][y][z];
	  (*pMesh).EltNodes[eid][1] = O1NMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][2] = O1NMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][3] = O1NMap[x + 1][y + 1][z];
	  (*pMesh).EltNodes[eid][4] = O1NMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][5] = O1NMap[x + 1][y][z + 1];
	  (*pMesh).EltNodes[eid][6] = O1NMap[x][y + 1][z + 1];
	  (*pMesh).EltNodes[eid][7] = O1NMap[x + 1][y + 1][z + 1];
	  */
	}
  }
  else
  {
    eid = 0;

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

	  /*
	  (*pMesh).EltNodes[eid][8] = O2XNMap[x][y][z];
	  (*pMesh).EltNodes[eid][9] = O2XNMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][10] = O2XNMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][11] = O2XNMap[x][y + 1][z + 1];
	  (*pMesh).EltNodes[eid][12] = O2YNMap[x][y][z];
	  (*pMesh).EltNodes[eid][13] = O2YNMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][14] = O2YNMap[x][y][z + 1];
	  (*pMesh).EltNodes[eid][15] = O2YNMap[x + 1][y][z + 1];
	  (*pMesh).EltNodes[eid][16] = O2ZNMap[x][y][z];
	  (*pMesh).EltNodes[eid][17] = O2ZNMap[x + 1][y][z];
	  (*pMesh).EltNodes[eid][18] = O2ZNMap[x][y + 1][z];
	  (*pMesh).EltNodes[eid][19] = O2ZNMap[x + 1][y + 1][z];
	  */
	}
  }

  threeDIntFree (O1NMap, size[0] + 2, size[1] + 2);
  threeDIntFree (O2XNMap, size[0] + 1, size[1] + 2);
  threeDIntFree (O2YNMap, size[0] + 2, size[1] + 1);
  threeDIntFree (O2ZNMap, size[0] + 2, size[1] + 2);

  SearchFoDNodes_cube (*pNodes, pFoDNodes);

  ut_free_1d_int (nqty);

  return;
}

void
BuildMMesh3DCentre (int sizeX, int sizeY, int sizeZ, int *pEltQty,
		    double ***pEltCoo)
{
  int x, y, z, eltid;

  (*pEltQty) = sizeX * sizeY * sizeZ;
  (*pEltCoo) = twoDDoubleDAlloc ((*pEltQty) + 1, 3);

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

/* WriteMMesh2D writes the mesh into file 'filename'. */
void
WriteMMesh2D (struct NODES Nodes, struct MESH Mesh, char *filename)
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
