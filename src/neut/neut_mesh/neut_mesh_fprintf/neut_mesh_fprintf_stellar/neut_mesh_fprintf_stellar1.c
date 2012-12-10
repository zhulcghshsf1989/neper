/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mesh_fprintf_stellar.h"

void
neut_mesh_fprintf_stellar (char* body, struct NODES Nodes, int* BoundNodes, struct MESH Mesh3D)
{
  FILE *file = NULL;
  char* stenode, *steele;

  stenode = ut_string_addextension (body, ".node");
  steele = ut_string_addextension (body, ".ele");

  file = ut_file_open (stenode, "w");
  neut_mesh_fprintf_stellar_nodes (file, BoundNodes, Nodes);
  ut_file_close (file, stenode, "w");

  file = ut_file_open (steele, "w");
  neut_mesh_fprintf_stellar_elts (file, Mesh3D);
  ut_file_close (file, steele, "w");

  return;
}

void
neut_mesh_fprintf_stellar_config (FILE* file)
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
