/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"WriteMeshAbq.h"

void
neut_mesh_fprintf_abq_nsets (FILE* file, struct NSET NSet0D,
                             struct NSET NSet1D, struct NSET NSet2D,
			     char* nset)
{
  int i, j, status;
  char** name;
  int qty;

  ut_string_separate (nset, ',', &name, &qty);

  for (i = 0; i < qty; i++)
  {
    status = -1;

    for (j = 1; j <= NSet0D.qty; j++)
      if (! strcmp (name[i], NSet0D.names[j]))
      {
	neut_mesh_fprintf_abq_nset (file, NSet0D.names[j], NSet0D.nodeqty[j], NSet0D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet1D.qty; j++)
      if (! strcmp (name[i], NSet1D.names[j]))
      {
	neut_mesh_fprintf_abq_nset (file, NSet1D.names[j], NSet1D.nodeqty[j], NSet1D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    for (j = 1; j <= NSet2D.qty; j++)
      if (! strcmp (name[i], NSet2D.names[j]))
      {
	neut_mesh_fprintf_abq_nset (file, NSet2D.names[j], NSet2D.nodeqty[j], NSet2D.nodes[j]);
	status = 0;
	break;
      }

    if (status == 0)
      continue;

    if (status == -1)
      ut_print_message (1, 3, "Nset %s was not found (skipped).\n", name[i]);
  }

  return;
}

void
neut_mesh_fprintf_abq_nset (FILE* file, char* name, int qty, int* nodeset)
{
  int i, col, nb;

  fprintf (file, "\n*Nset, nset=%s\n", name);

  col = 0;
  nb = 0;
  for (i = 0; i < qty - 1; i++)
  {
    ut_print_wnc (file, &col, 72, "%d, ", nodeset[i]);
    if (++nb == 16)
    {
      fprintf (file, "\n");
      col = 0;
      nb = 0;
    }
  }
  ut_print_wnc (file, &col, 72, "%d\n", nodeset[qty - 1]);

  return;
}
