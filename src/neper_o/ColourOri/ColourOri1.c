/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"ColourOri.h"

void
ColourOri (struct IN In, struct ORI Ori)
{
  int i, j;
  FILE* file;
  double* R = ol_R_alloc ();
  double SQRT2 = 1.41421356237309504880;
  int* rgb = ut_alloc_1d_int (3);

  file = ut_file_open (In.col, "w");

  if (strcmp (In.colourspec, "R") == 0)
  {
    for (i = 1; i <= Ori.N; i++)
    {
      ol_q_R (Ori.q[i], R);
      ol_R_Rcrysym (R, In.crysym, R);
      for (j = 0; j < 3; j++)
	rgb[j] = ut_num_d2ri (255 * (R[j] + (SQRT2 - 1)) / (2 * (SQRT2 - 1)));

      ut_array_1d_int_fprintf (file, rgb, 3, "%3d");
    }
  }
  else
    ut_error_reportbug ();

  ut_file_close (file, In.col, "w");

  ol_R_free (R);
  ut_free_1d_int (rgb);

  return;
}
