/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshGeof.h"

void
neut_mesh_fprintf_geof_nset (FILE* file, char* name, int nodeqty, int* nset)
{
  int i, col;

  fprintf (file, "\n**nset %s\n", name);

  col = 0;
  for (i = 0; i < nodeqty; i++)
    ut_print_wnc_int (file, nset[i], &col, 72);

  fprintf (file, "\n");

  return;
}
    
void
neut_mesh_fprintf_geof_part_nset (FILE* file, struct PART Part)
{
  int p, i, col;

  for (p = 0; p < Part.qty; p++)
  {
    fprintf (file, "\n**nset part%d\n", p + 1);
    col = 0;
    for (i = 0; i < Part.node_parts[0]; i++)
      if (Part.node_parts[i] == p)
	FPrintfWNC (file, i, &col, 72);

    fprintf (file, "\n");
  }
  
  return;
}
  
void
neut_mesh_fprintf_geof_part_elset (FILE* file, struct PART Part)
{
  int p, i, col;

  for (p = 0; p < Part.qty; p++)
  {
    fprintf (file, "\n**elset part%d\n", p + 1);
    col = 0;
    for (i = 0; i < Part.elt_parts[0]; i++)
      if (Part.elt_parts[i] == p)
	FPrintfWNC (file, i, &col, 72);

    fprintf (file, "\n");
  }
  
  return;
}
