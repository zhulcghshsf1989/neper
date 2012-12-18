/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "PointPoly.h"

void
PrintPointPoly (struct IN In, struct TESS Tess)
{
  int i, j, id;
  int qty = 0;
  FILE *file = NULL;
  double **points = NULL;

  /* recording points */
  if (In.printpointpoly == 1)
  {
    qty = ut_num_d2ri (ut_file_nbwords (In.point) / 3.);
    points = ut_alloc_2d (qty, 3);

    file = ut_file_open (In.point, "r");
    ut_array_2d_fscanf (file, points, qty, 3);
    ut_file_close (file, In.point, "r");
  }
  else
    return;

  ut_print_message (0, 3, "Writing point polyhedron identifiers...\n");
  /* searching point poly id */
  file = ut_file_open (In.polyid, "w");

  id = 0;
  for (i = 0; i < qty; i++)
  {
    for (j = 1; j <= Tess.PolyQty; j++)
    {
      id = 0;
      if (neut_tess_point_inpoly (Tess, points[i], j) == 1)
      {
	id = j;
	break;
      }
    }

    fprintf (file, "%d\n", id);
  }

  ut_free_2d (points, qty);
  ut_file_close (file, In.polyid, "w");

  return;
}
