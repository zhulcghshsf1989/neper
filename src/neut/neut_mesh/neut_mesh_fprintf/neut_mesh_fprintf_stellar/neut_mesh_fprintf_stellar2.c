/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mesh_fprintf_stellar_lcl.h"

void
neut_mesh_fprintf_stellar_nodes (FILE* file, int* BoundNodes, struct NODES Nodes)
{
  int i, j, bound;

  fprintf (file, "%d 3 0 1\n", Nodes.NodeQty);

  for (i = 1; i <= Nodes.NodeQty; i++)
  {
    fprintf (file, "%d", i);
    for (j = 0; j < 3; j++)
      fprintf (file, "  %.12f", Nodes.NodeCoo[i][j]);

    bound = 0;
    if (BoundNodes != NULL)
      if (ut_array_1d_int_eltpos (BoundNodes + 1, BoundNodes[0], i) != -1)
	bound = 1;

    fprintf (file, " %d\n", bound);
  }

  return;
}

void
neut_mesh_fprintf_stellar_elts (FILE* file, struct MESH Mesh)
{
  int i;

  fprintf (file, "%d 4 0\n", Mesh.EltQty);

  for (i = 1; i <= Mesh.EltQty; i++)
  {
    fprintf (file, "%d ", i);
    fprintf (file, " %d %d %d %d",
	Mesh.EltNodes[i][0], Mesh.EltNodes[i][1],
	Mesh.EltNodes[i][3], Mesh.EltNodes[i][2]);
    fprintf (file, "\n");
  }

  return;
}
