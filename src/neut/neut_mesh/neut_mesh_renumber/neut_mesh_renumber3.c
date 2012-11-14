/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mesh_renumber.h"

void
neut_nodes_switch (struct NODES *pNodes, int n1, int n2)
{
  double* coo = ut_alloc_1d (3);

  ut_array_2d_switchlines ((*pNodes).NodeCoo, 3, n1, n2);

  ut_free_1d (coo);

  return;
}

void
neut_mesh_switch (struct MESH *pMesh, int n1, int n2)
{
  double* coo = ut_alloc_1d (3);
  int eltnodeqty = neut_elt_nodeqty ((*pMesh).EltType, (*pMesh).Dimension, (*pMesh).EltOrder);

  ut_array_2d_int_switchlines ((*pMesh).EltNodes, eltnodeqty, n1, n2);

  ut_free_1d (coo);

  return;
}
