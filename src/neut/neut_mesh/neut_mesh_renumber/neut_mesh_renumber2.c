/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mesh_renumber.h"

void
neut_nodes_sortbynumber (struct NODES *pNodes, int* node_nbs)
{
  int i;

  for (i = 1; i <= (*pNodes).NodeQty; i++)
    neut_nodes_switch (pNodes, i, node_nbs[i]);

  return;
}

void
neut_mesh_sortbynumber (struct MESH *pMesh, int* elt_nbs)
{
  int i;

  for (i = 1; i <= (*pMesh).EltQty; i++)
    neut_mesh_switch (pMesh, i, elt_nbs[i]);

  return;
}

void
neut_mesh_renumber (struct MESH *pMesh, int *nodes_old_new, int *elts_old_new, int* elsets_old_new)
{
  int i, j, eltnodeqty;

  /* correcting elt nodes */
  if (nodes_old_new != NULL)
  {
    eltnodeqty =
      neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

    for (i = 1; i <= (*pMesh).EltQty; i++)
      for (j = 0; j < eltnodeqty; j++)
	(*pMesh).EltNodes[i][j] = nodes_old_new[(*pMesh).EltNodes[i][j]];
  }

  /* correcting elset elts */
  if (elts_old_new != NULL)
    for (i = 1; i <= (*pMesh).ElsetQty; i++)
      for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
	(*pMesh).Elsets[i][j] = elts_old_new[(*pMesh).Elsets[i][j]];

  /* correcting elt elsets */
  if (elsets_old_new != NULL)
    for (i = 1; i <= (*pMesh).EltQty; i++)
      (*pMesh).EltElset[i] = elsets_old_new[(*pMesh).EltElset[i]];

  return;
}

/*
void
neut_mesh_renumber_bypoly (int* sequence, struct NODES* pNodes,
                           struct MESH* pMesh2D, struct MESH* pMesh)
{
  int i, j, k, neweltid, newnodeid, elsetidinv, eltid, eltidinv, nodeid;
  int eltnodeqty;
  int* elts_old_new;
  int* nodes_old_new;

  ut_array_1d_int_set (nodes_old_new, (*pNodes).nodenbMax + 1, -1);

  eltnodeqty =
    neut_elt_nodeqty ("tri", (*pMesh).Dimension, (*pMesh).EltOrder);


  neweltid = (*pMesh).EltNb[1] - 1;
  newnodeid = (*pNodes).nodenb[1] - 1;

  for (i = 1; i <= sequence[0]; i++)
  {
    elsetidinv = sequence[i];

    for (j = 1; j <= (*pMesh).Elsets[elsetidinv][0]; j++)
    {
      eltid    = (*pMesh).Elsets[elsetidinv][j];
      eltidinv = (*pMesh).EltNbInv[eltid];

      elts_old_new [eltid] = ++neweltid;

      for (k = 0; k < eltnodeqty; k++)
      {
	nodeid = (*pMesh).EltNodes[eltidinv][k];

	if (nodes_old_new[nodeid] == -1)
	  nodes_old_new [nodeid] = ++newnodeid;
      }
    }
  }

  if (neweltid != (*pMesh).EltQty || newnodeid != (*pNodes).NodeQty)
  {
    printf ("bad elt and / or node renumbering\n");
    printf ("neweltid = %d != (*pMesh).EltQty = %d\n", neweltid, (*pMesh).EltQty);
    printf ("newnodeid = %d != (*pNodes).NodeQty = %d\n", newnodeid, (*pNodes).NodeQty);

    abort ();
  }

  neut_nodes_renumber (pNodes, nodes_old_new);
  neut_mesh_renumber (pMesh, nodes_old_new, elts_old_new);
  ut_free_1d_int (elts_old_new);
  
  elts_old_new = ut_alloc_1d_int ((*pMesh2D).EltNbMax + 1);
  if ((*pMesh2D).EltQty > 0)
    neut_mesh_renumber_continuous (pMesh2D, nodes_old_new, elts_old_new);

  neut_nodes_sortbynumber (pNodes);
  neut_mesh_sortbynumber (pMesh);
  
  ut_free_1d_int (elts_old_new);
  ut_free_1d_int (nodes_old_new);

  return;
}
*/
