/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_mesh_fprintf_fepx.h"

void
neut_mesh_fprintf_fepx_nset (FILE * file, char *name, int qty, int *nset)
{
  int i;

  for (i = 0; i < qty; i++)
    fprintf (file, "%d %s\n", nset[i] - 1, name);

  return;
}
