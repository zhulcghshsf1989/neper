/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteRemesh.h"

void
WriteRemesh (struct IN In, struct MESH Mesh3D, int* remesh_elt_oldelt)
{
  int i;
  FILE* file;

  file = ut_file_open (In.rem1, "w");

  for (i = 1; i <= Mesh3D.EltQty; i++)
    fprintf (file, "%d %d\n", i, remesh_elt_oldelt[i]);
  
  ut_file_close (file, In.rem1, "w");
  
  return;
}
