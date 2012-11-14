/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing0D.h"

void
Meshing0D (struct GEO Geo, struct GEOPARA GeoPara, struct NODES* pNodes, struct MESH* pMesh0D)
{
  int i;
  struct NODES N;
  struct MESH M;
  char* message = ut_alloc_1d_char (8);

  neut_nodes_set_zero (&N);
  neut_mesh_set_zero (&M);
  (*pMesh0D).Dimension = 0;
  (*pMesh0D).EltOrder = 1;
  (*pMesh0D).EltType = ut_alloc_1d_char (5);
  sprintf ((*pMesh0D).EltType, "tri");

  if (Geo.maxff > 0)
  {
    if (1.01 * Geo.sel < GeoPara.cl / GeoPara.pcl)
      ut_print_messagewnc (1, 72, "Regularization was used with sel < cl / pl.  The mesh could be locally overrefined.");
    if (Geo.dbound != NULL)
      if (1.01 * Geo.dboundsel < GeoPara.dboundcl / GeoPara.dboundpcl)
	ut_print_messagewnc (1, 72, "Regularization was used with dboundsel < dboundcl / dboundpl.  The mesh could be locally overrefined.");
  }

  ut_print_message (0, 2, "0D meshing ... ");

  ut_print_progress (stdout, 0, Geo.VerQty, "%3.0f%%", message);
  for (i = 1; i <= Geo.VerQty; i++)
  {
    VerMeshing (Geo, i, GeoPara, &N, &M);

    /* renumbering mesh nodes / elts to match global nodes */
    M.EltNodes[1][0] = (*pNodes).NodeQty + 1;
    M.Elsets[1][1] = (*pNodes).NodeQty + 1;

    neut_nodes_addnode (pNodes, N.NodeCoo[1], N.NodeCl[1]);
    neut_mesh_addelt (pMesh0D, M.EltNodes[1]);
    neut_mesh_addelset (pMesh0D, M.Elsets[1] + 1, M.Elsets[1][0]);

    ut_print_progress (stdout, i, Geo.VerQty, "%3.0f%%", message);
  }

  neut_nodes_free (&N);
  neut_mesh_free (&M);
  ut_free_1d_char (message);

  return;
}
