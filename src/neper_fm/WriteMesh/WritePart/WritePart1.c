/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WritePart.h"


void
neut_part_fprintf (struct IN In, struct PART Part)
{
  int i;
  FILE* file;

  file = ut_file_open (In.npart, "w");
  for (i = 1; i <= Part.node_parts[0]; i++)
    fprintf (file, "%d\n", Part.node_parts[i] + 1);
  ut_file_close (file, In.npart, "w");

  file = ut_file_open (In.epart, "w");
  for (i = 1; i <= Part.elt_parts[0]; i++)
    fprintf (file, "%d\n", Part.elt_parts[i] + 1);
  ut_file_close (file, In.epart, "w");

  return;
}
