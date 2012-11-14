/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "MMeshUtils.h"

void
FoDAddEltNode (int **FoDNodes, int face, struct MESH Mesh, int *ppos,
	       int x, int y, int z, int XEQty, int YEQty, int eltnode)
{
  int eltid, nodeid;

  eltid = EltCoo2Id (x, y, z, XEQty, YEQty);
  nodeid = Mesh.EltNodes[eltid][eltnode];
  (*ppos)++;
  FoDNodes[face][(*ppos)] = nodeid;

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

/*
int GetNodeFromElt(struct MESH Mesh,int x,int y,int z,int size,int pos)
{
    int neltid;

    neltid=EltCoo2Id(x,y,z,XEQty,YEQty);

    return Mesh.EltNodes[neltid][pos];
}
*/

int
EltCoo2Id (int X, int Y, int Z, int XEQty, int YEQty)
{
  return (Z - 1) * YEQty * XEQty + (Y - 1) * XEQty + X;
}

int
PointElt (double *Coo, int msize)
{
  int i, elt;
  int *EltPos = ut_alloc_1d_int (3);

  for (i = 0; i <= 2; i++)
    if (Coo[i] < 0 || Coo[i] > 1)
      return -1;

  printf ("coo: %f %f %f\n", Coo[0], Coo[1], Coo[2]);
  for (i = 0; i <= 2; i++)
    EltPos[i] = ceil (Coo[i] * (double) msize);

  elt = EltCoo2Id (EltPos[0], EltPos[1], EltPos[2], msize, msize);

  oneDIntFree (EltPos);

  return elt;
}
