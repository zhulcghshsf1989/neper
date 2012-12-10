/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fscanf.h"

void
neut_mesh_fscanf_msh (FILE * msh, struct NODES *pNodes, struct MESH
    *pMesh0D, struct MESH *pMesh1D, struct MESH *pMesh2D, struct MESH
    *pMesh3D)
{
  int *node_nbs = NULL;
  int leftelts;
  struct MESH Trash;
  int contiguous;

  neut_mesh_set_zero (&Trash);
  neut_nodes_free (pNodes);

  neut_mesh_fscanfHead (msh, &contiguous);
  (*pNodes).NodeQty = ReadNodesHead (msh);

  if (contiguous == 0)
  {
    node_nbs = ut_alloc_1d_int ((*pNodes).NodeQty + 1);
    node_nbs[0] = (*pNodes).NodeQty;
  }
  ReadNodes (msh, pNodes, node_nbs);
  // neut_nodes_renumber_continuous (pNodes, node_nbs, &nodes_old_new);
  
  ut_file_skip (msh, 1);
  if (fscanf (msh, "%d", &leftelts) != 1)
    abort ();

  if (pMesh0D != NULL)
    leftelts -= ReadMeshOfDim (msh, pMesh0D, node_nbs, 0, leftelts);
  else
    leftelts -= ReadMeshOfDim (msh, &Trash, node_nbs, 0, leftelts);

  if (pMesh1D != NULL)
    leftelts -= ReadMeshOfDim (msh, pMesh1D, node_nbs, 1, leftelts);
  else
    leftelts -= ReadMeshOfDim (msh, &Trash, node_nbs, 1, leftelts);

  if (pMesh2D != NULL)
    leftelts -= ReadMeshOfDim (msh, pMesh2D, node_nbs, 2, leftelts);
  else
    leftelts -= ReadMeshOfDim (msh, &Trash, node_nbs, 2, leftelts);

  if (pMesh3D != NULL)
    leftelts -= ReadMeshOfDim (msh, pMesh3D, node_nbs, 3, leftelts);
  else
    leftelts -= ReadMeshOfDim (msh, &Trash, node_nbs, 3, leftelts);

  neut_mesh_free (&Trash);

  ut_free_1d_int (node_nbs);

  if (leftelts != 0)
  {
    ut_print_message (2, 0, "Reading of mesh file failed!\n");
    ut_print_message (2, 0, "(Element quantity does not match.)\n");
    printf ("leftelts = %d\n", leftelts);
    abort ();
  }

  neut_nodes_init_boundingbox (pNodes);

  ReadEltsFoot (msh);

  return;
}
