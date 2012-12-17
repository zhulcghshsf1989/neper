/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_mesh_stellar.h"

void
nem_mesh_3d_improve_stellar (struct NODES* pN, struct MESH* pM, int skinnodeqty)
{
  int i, j, id, qty;
  FILE* file;
  FILE* file2;
  int* skin = NULL;
  double** skincoo = NULL;
  int* node_nbs = NULL;
  int* node_nbs_inv = NULL;
  double* dist = NULL;

  skin = ut_alloc_1d_int (skinnodeqty + 1);
  skincoo = ut_alloc_2d (skinnodeqty + 1, 3);

  for (i = 1; i <= skinnodeqty; i++)
    ut_array_1d_memcpy (skincoo[i], 3, (*pN).NodeCoo[i]);

  skin[0] = skinnodeqty;
  for (i = 1; i <= skinnodeqty; i++)
    skin[i] = i;

  file = ut_file_open ("Stellar.config", "W");
  neut_mesh_fprintf_stellar_config (file);
  ut_file_close (file, "Stellar.config", "W");

  neut_mesh_fprintf_stellar ("tmp", *pN, skin, *pM);

  if (system ("Stellar -s Stellar.config -L 0 tmp > /dev/null") == -1)
    abort ();

  neut_nodes_free (pN);
  neut_mesh_free (pM);

  file = ut_file_open ("tmp.1.node", "R");
  file2 = ut_file_open ("tmp.1.ele", "R");
  neut_mesh_fscanf_stellar (file, file2, pN, pM);
  ut_file_close (file, "tmp.1.node", "R");
  ut_file_close (file2, "tmp.1.ele", "R");

  node_nbs = ut_alloc_1d_int ((*pN).NodeQty + 1);

/*----------------------------------------------------------------------
 * the skin nodes are search out and put at the beginning. Everything is
 * renumber accordingly */

  dist = ut_alloc_1d ((*pN).NodeQty + 1);
  for (i = 1; i <= skinnodeqty; i++)
  {
    for (j = 1; j <= (*pN).NodeQty; j++)
      dist[j] = ut_space_dist (skincoo[i], (*pN).NodeCoo[j]);

    id = 1 + ut_array_1d_min_index (dist + 1, (*pN).NodeQty);
    if (fabs (dist[id]) > 1e-12)
    {
      ut_print_message (2, 0, "neut_mesh_3d_improve_stellar: skin node not found\n");
      abort ();
    }
    node_nbs[i] = id;
    node_nbs[0]++;
  }
  ut_free_1d (dist);

  for (i = skinnodeqty + 1; i <= (*pN).NodeQty; i++)
  {
    for (j = 1; j <= (*pN).NodeQty; j++)
      if (ut_array_1d_int_eltpos (node_nbs + 1, (*pN).NodeQty, j) == -1)
	node_nbs[++node_nbs[0]] = j;
  }
  
  if (node_nbs[0] != (*pN).NodeQty)
    abort ();

  ut_array_1d_int_inv (node_nbs + 1, node_nbs[0], &node_nbs_inv, &qty);
  ut_array_1d_int_addval (node_nbs_inv + 1, (*pN).NodeQty, 1, node_nbs_inv + 1);
  node_nbs_inv[0] = node_nbs[0];
  
  neut_nodes_switch (pN, node_nbs_inv);
  neut_mesh_nodes_switch (pM, node_nbs_inv);

  /*
  printf ("AFTER\n");
  neut_debug_nodes (stdout, *pN);
  neut_debug_mesh  (stdout, *pM);
  */

  ut_free_1d_int (node_nbs);
  ut_free_1d_int (node_nbs_inv);
  ut_free_1d_int (skin);
  ut_free_2d (skincoo, skinnodeqty + 1);

  return;
}
