/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nefm_mesh_stellar.h"

void
nefm_mesh_fprintf_stellar_config (FILE* file)
{
  fprintf(file, "verbosity 0\n");
  fprintf(file, "usecolor 0\n");
  fprintf(file, "outputandquit 0\n");
  fprintf(file, "qualmeasure 5\n");
  fprintf(file, "sinewarpfactor 0 0.75\n");
  fprintf(file, "goalanglemin 0 90.0\n");
  fprintf(file, "goalanglemax 0 90.0\n");
  fprintf(file, "nonsmooth 0\n");
  fprintf(file, "facetsmooth 0\n");
  fprintf(file, "segmentsmooth 0\n");
  fprintf(file, "usequadrics 0\n");
  fprintf(file, "quadricoffset 0 0.8\n");
  fprintf(file, "quadricscale 0 300.0\n");
  fprintf(file, "flip22 0\n");
  fprintf(file, "multifaceremoval 0\n");
  fprintf(file, "singlefaceremoval 0\n");
  fprintf(file, "edgeremoval 0\n");
  fprintf(file, "edgecontraction 0\n");
  fprintf(file, "enableinsert 0\n");
  fprintf(file, "insertbody 0\n");
  fprintf(file, "insertfacet 0\n");
  fprintf(file, "insertsegment 0\n");
  fprintf(file, "insertthreshold 0 0.031\n");
  fprintf(file, "sizing 0\n");
  fprintf(file, "sizingpass 0\n");
  fprintf(file, "targetedgelength 0 0.0\n");
  fprintf(file, "longerfactor 0 2.0\n");
  fprintf(file, "shorterfactor 0 0.50\n");
  fprintf(file, "anisotropic 0\n");
  fprintf(file, "tensor 6\n");
  fprintf(file, "minsineout 0\n");
  fprintf(file, "minangout 0\n");
  fprintf(file, "maxangout 0\n");
  fprintf(file, "vlrmsout 0\n");
  fprintf(file, "nrrout 0\n");
  fprintf(file, "animate 0\n");
  fprintf(file, "timeseries 0\n");

  return;
}

void
nefm_mesh_fprintf_stellar_nodes (FILE* file, int* BoundNodes, struct NODES Nodes)
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
nefm_mesh_fprintf_stellar_elts (FILE* file, struct MESH Mesh)
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
