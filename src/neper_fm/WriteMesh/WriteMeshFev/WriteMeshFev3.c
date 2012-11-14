/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshFev.h"

void
neut_mesh_fprintf_fev_nset (FILE * file, char *name, int qty, int *nset)
{
  int i;

  for (i = 0; i < qty; i++)
    fprintf (file, "%d %s\n", nset[i] - 1, name);

  return;
}
