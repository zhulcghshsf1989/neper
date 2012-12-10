/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_part.h"

void
neut_part_set_zero (struct PART* pPart)
{
  (*pPart).qty = 0;
  (*pPart).node_parts = NULL;
  (*pPart).elt_parts = NULL;
  (*pPart).nodeqty = NULL;
  (*pPart).eltqty = NULL;
  (*pPart).targeteltqty = NULL;

  return;
}

void
neut_part_free (struct PART Part)
{
  ut_free_1d_int (Part.node_parts);
  ut_free_1d_int (Part.elt_parts);
  ut_free_1d_int (Part.nodeqty);
  ut_free_1d_int (Part.eltqty);
  ut_free_1d_int (Part.targeteltqty);

  return;
}

void
neut_part_fprintf (char* body, struct PART Part)
{
  int i;
  FILE* file = NULL;
  char *npart, *epart;

  npart = ut_string_addextension (body, ".npart");
  epart = ut_string_addextension (body, ".epart");

  file = ut_file_open (npart, "w");
  for (i = 1; i <= Part.node_parts[0]; i++)
    fprintf (file, "%d\n", Part.node_parts[i] + 1);
  ut_file_close (file, npart, "w");

  file = ut_file_open (epart, "w");
  for (i = 1; i <= Part.elt_parts[0]; i++)
    fprintf (file, "%d\n", Part.elt_parts[i] + 1);
  ut_file_close (file, epart, "w");

  return;
}
