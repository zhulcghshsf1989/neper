/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf_stellar_lcl.h"

void
neut_mesh_fscanf_stellar (FILE * nodes, FILE * elts,
			  struct NODES *pNodes, struct MESH *pMesh)
{
  int i, eltnodeqty, bound;
  int *node_nbs = NULL;
  int *elt_nbs = NULL;

  /* nodes */
  if (fscanf (nodes, "%d", &((*pNodes).NodeQty)) != 1)
    abort ();

  node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
  node_nbs[0] = (*pNodes).NodeQty;
  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  if (fscanf (nodes, "%d%d%d", &bound, &bound, &bound) != 3)
    abort ();

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (fscanf (nodes, "%d", &(node_nbs[i])) != 1)
      abort ();

    ut_array_1d_fscanf (nodes, (*pNodes).NodeCoo[i], 3);
    if (fscanf (nodes, "%d", &bound) != 1)
      abort ();
  }

  /* reading elements ----------------------------------------------- */
  (*pMesh).Dimension = 3;

  if (fscanf (elts, "%d%d", &((*pMesh).EltQty), &eltnodeqty) != 2)
    abort ();

  if (eltnodeqty == 4)
    (*pMesh).EltOrder = 1;
  else if (eltnodeqty == 10)
    (*pMesh).EltOrder = 2;
  else
    abort ();

  elt_nbs = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).EltNodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  ut_file_skip (elts, 1);
  for (i = 1; i <= (*pMesh).EltQty; i++)
  {
    if (fscanf (elts, "%d", &(elt_nbs[i])) != 1)
      abort ();
    ut_array_1d_int_fscanf (elts, (*pMesh).EltNodes[i], eltnodeqty);
  }

  (*pMesh).Elsets = ut_alloc_1d_pint (2);
  (*pMesh).Elsets[1] = ut_alloc_1d_int ((*pMesh).EltQty + 1);
  (*pMesh).Elsets[1][0] = (*pMesh).EltQty;
  for (i = 1; i <= (*pMesh).EltQty; i++)
    (*pMesh).Elsets[1][i] = i;

  return;
}
