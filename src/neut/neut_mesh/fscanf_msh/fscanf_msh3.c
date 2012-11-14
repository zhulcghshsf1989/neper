/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"fscanf_msh.h"

int
ReadNodesHead (FILE * msh)
{
  int nodeqty;

  ut_file_skip (msh, 1);

  fscanf (msh, "%d", &nodeqty);
  if (nodeqty == 0)
  {
    ut_print_message (2, 0, "Mesh has no nodes!\n");
    abort ();
  }

  return nodeqty;
}

void
ReadNodesFoot (FILE * msh)
{
  char foot[1000];

  fscanf (msh, "%s", foot);

  if (strcmp (foot, "$EndNodes") != 0)
    ut_print_message (2, 0, "Reading msh file: error.\n");

  return;
}

void
ReadNodesProp (FILE * msh, struct NODES *pNodes, int *node_nbs)
{
  int i, j, tmp;
  int contiguous = 0;

  if (node_nbs == NULL)
    contiguous = 1;

  (*pNodes).NodeCoo = ut_alloc_2d ((*pNodes).NodeQty + 1, 3);

  for (i = 1; i <= (*pNodes).NodeQty; i++)
  {
    if (contiguous == 0)
      fscanf (msh, "%d", &(node_nbs[i]));
    else
      fscanf (msh, "%d", &tmp);

    for (j = 0; j < 3; j++)
      fscanf (msh, "%lf", &(*pNodes).NodeCoo[i][j]);
  }

  return;
}

int
ReadMeshOfDim (FILE * msh, struct MESH *pMesh, int *node_nbs,
	       int Dimension, int MaxEltQty)
{
  int *elt_nbs = NULL;
  int *elset_nbs = NULL;
  int contiguous = 0;

  if (node_nbs == NULL)
    contiguous = 1;

  neut_mesh_free (pMesh);

  if (Dimension < 0 || Dimension > 3)
  {
    ut_print_message (2, 0, "Wrong mesh dimension: %d!\n", Dimension);
    abort ();
  }

  if (MaxEltQty != 0)
  {
    (*pMesh).Dimension = Dimension;

    if (contiguous == 0)
      ReadEltsProp (msh, pMesh, &elt_nbs, MaxEltQty);
    else
      ReadEltsProp (msh, pMesh, NULL, MaxEltQty);

    if (contiguous == 0)
      SetElsets (pMesh, elt_nbs, &elset_nbs);
    else
      SetElsets (pMesh, NULL, NULL);

    if (contiguous == 0)
      neut_mesh_renumber_continuous (pMesh, node_nbs, elt_nbs, elset_nbs);
  }
  
  ut_free_1d_int (elt_nbs);
  ut_free_1d_int (elset_nbs);

  return (*pMesh).EltQty;
}

void
ReadEltsFoot (FILE * msh)
{
  char foot[1000];

  fscanf (msh, "%s", foot);

  if (strcmp (foot, "$EndElements") != 0)
    ut_print_message (2, 0, "Reading msh file: error.\n");

  return;
}
