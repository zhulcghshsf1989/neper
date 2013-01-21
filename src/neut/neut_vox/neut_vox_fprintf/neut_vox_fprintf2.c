/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_vox_fprintf_lcl.h"

void
neut_vox_fprintf_head (FILE* file, char* format, struct VOX Vox)
{
  fprintf (file, "***vox\n");
  fprintf (file, " **format\n");
  fprintf (file, " 1.10 %s\n", format);
  fprintf (file, " **general\n");
  fprintf (file, " %d %d %d\n", Vox.size[0], Vox.size[1], Vox.size[2]);
  fprintf (file, " %.12f %.12f %.12f\n", Vox.vsize[0], Vox.vsize[1], Vox.vsize[2]);

  return;
}

void
neut_vox_fprintf_data (FILE* file, char* format, struct VOX Vox)
{
  int i, j, k, count;
  // char* progress = ut_alloc_1d_char (10);
  
  fprintf (file, " **voxel\n");

  if (! strcmp (format, "ascii"))
  {
    count = 0;
    for (k = 1; k <= Vox.size[2]; k++)
    {
      // ut_print_progress (stdout, k + 1, Vox.size[2], "%3.0f%%", progress);
      for (j = 1; j <= Vox.size[1]; j++)
	for (i = 1; i <= Vox.size[0]; i++)
	  ut_print_wnc_int (file, Vox.VoxPoly[i][j][k], &count, 72);
    }
  }
  else if (! strncmp (format, "binary", 6))
  {
    int size;
  
    if (! strcmp (format, "binary8"))
      size = sizeof (unsigned char);
    else if (! strcmp (format, "binary16"))
      size = sizeof (short);
    else if (! strcmp (format, "binary32"))
      size = sizeof (int);
    else
      abort ();

    for (k = 1; k <= Vox.size[2]; k++)
    {
      // ut_print_progress (stdout, k + 1, Vox.size[2], "%3.0f%%", progress);
      for (j = 1; j <= Vox.size[1]; j++)
	for (i = 1; i <= Vox.size[0]; i++)
	  fwrite (&(Vox.VoxPoly[i][j][k]), size, 1, file);
    }
  }

  // ut_free_1d_char (progress);

  return;
}

void
neut_vox_fprintf_foot (FILE* file)
{
  fprintf (file, "\n***end\n");

  return;
}
