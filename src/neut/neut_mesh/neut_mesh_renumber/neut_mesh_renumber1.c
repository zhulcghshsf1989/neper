/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mesh_renumber.h"

/* node_nbs[...] = node qty then new pos */
/* RAM could be improved by more elegant exchange of lines (not through
 * a big copy coo array */
void
neut_nodes_renumber_switch (struct NODES *pNodes, int* node_nbs)
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

/* RAM could be improved by more elegant exchange of lines (not through
 * a big copy eltnodes array */
void
neut_mesh_renumber_switch (struct MESH *pMesh, int* elt_nbs)
{
  int i, j;
  int** eltnodes = NULL;
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  eltnodes = ut_alloc_2d_int ((*pMesh).EltQty + 1, eltnodeqty);

  for (i = 1; i <= elt_nbs[0]; i++)
    ut_array_1d_int_memcpy (eltnodes[i], eltnodeqty, (*pMesh).EltNodes[i]);

  for (i = 1; i <= (*pMesh).EltQty; i++)
    ut_array_1d_int_memcpy ((*pMesh).EltNodes[elt_nbs[i]],
	                eltnodeqty, eltnodes[i]);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    for (j = 1; j <= (*pMesh).Elsets[i][0]; j++)
      (*pMesh).Elsets[i][j] = elt_nbs[(*pMesh).Elsets[i][j]];

  if ((*pMesh).NodeElts != NULL)
  {
    int nodeqty = (*pMesh).NodeElts[0][0];
    ut_free_2d_int ((*pMesh).NodeElts, nodeqty + 1);
    neut_mesh_init_nodeelts (pMesh, nodeqty);

    // the code below would be a little faster but does not work
    /*
    for (i = 1; i <= (*pMesh).NodeElts[0][0]; i++)
      for (j = 1; j <= (*pMesh).NodeElts[i][0]; j++)
	(*pMesh).NodeElts[i][j] = elt_nbs[(*pMesh).NodeElts[i][j]];
    */
  }

  ut_free_2d_int (eltnodes, (*pMesh).EltQty);

  return;
}

/* node_nbs[...] = node qty then new pos */
void
neut_nodes_renumber_switch_mesh (struct MESH* pMesh, int* node_nbs)
{
  int i;
  int j, eltnodeqty;

  eltnodeqty =
    neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  /* neut_mesh_renumber (pMesh, node_nbs, NULL); */
  for (i = 1; i <= (*pMesh).EltQty; i++)
    for (j = 0; j < eltnodeqty; j++)
      (*pMesh).EltNodes[i][j] = node_nbs[(*pMesh).EltNodes[i][j]];

  return;
}

void
neut_mesh_renumber_continuous (struct MESH *pMesh, int* node_nbs,
                               int* elt_nbs, int* elset_nbs)
{
  int i, elt_nb_max, elset_nb_max;
  int* nodes_old_new = NULL;
  int* elts_old_new = NULL;
  int* elsets_old_new = NULL;
  
  int node_nb_max = ut_array_1d_int_max (node_nbs + 1, node_nbs[0]);

  if (node_nbs != NULL)
  {
    nodes_old_new = ut_alloc_1d_int (node_nb_max + 1);
    for (i = 1; i <= node_nbs[0]; i++)
      nodes_old_new[node_nbs[i]] = i;
  }

  if (elt_nbs != NULL)
  {
    elt_nb_max = ut_array_1d_int_max (elt_nbs + 1, (*pMesh).EltQty);

    elts_old_new = ut_alloc_1d_int (elt_nb_max + 1);
    for (i = 1; i <= (*pMesh).EltQty; i++)
      elts_old_new[elt_nbs[i]] = i;
    
    elset_nb_max = ut_array_1d_int_max (elset_nbs + 1, (*pMesh).ElsetQty);

    elsets_old_new = ut_alloc_1d_int (elset_nb_max + 1);
    for (i = 1; i <= elset_nbs[0]; i++)
      elsets_old_new[elset_nbs[i]] = i;

    neut_mesh_renumber (pMesh, nodes_old_new, elts_old_new, elsets_old_new);
  }

  ut_free_1d_int (nodes_old_new);
  ut_free_1d_int (elts_old_new);
  ut_free_1d_int (elsets_old_new);
  
  return;
}

/* renumber_continuous must have been called before */
/*
void
neut_mesh_renumber_crude (struct NODES *pNodes, struct MESH* pMesh2D, struct MESH *pMesh)
{
  int i, size;
  double** GrainCentre = ut_alloc_2d ((*pMesh).ElsetQty + 1, 3);
  int* sequence = ut_alloc_1d_int ((*pMesh).ElsetQty + 1);

  if ((*pNodes).BBox == NULL)
    neut_nodes_init_boundingbox (pNodes);

  for (i = 1; i <= (*pMesh).ElsetQty; i++)
    neut_mesh_elsetcentre (*pNodes, *pMesh, (*pMesh).ElbsetNb[i], GrainCentre[i]);

  size = ut_num_d2ri (pow ((*pMesh).ElsetQty, 0.33333));
  ut_space_arrangepoints (GrainCentre + 1, (*pMesh).ElsetQty, (*pNodes).BBox, size, sequence);
  ut_array_1d_int_addval (sequence + 1, sequence[0], 1, sequence + 1);

  neut_mesh_renumber_bypoly (sequence, pNodes, pMesh2D, pMesh);

  ut_free_1d_int (sequence);
  ut_free_2d (GrainCentre, (*pMesh).ElsetQty + 1);

  return;
}
*/

