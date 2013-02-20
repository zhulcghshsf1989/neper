/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_vox_fscanf_lcl.h"

/* Tessellation exportation: head */
void
neut_vox_fscanf_head (struct VOX* pVox, char** pformat, FILE * file)
{
  int status;

  (*pformat) = ut_alloc_1d_char (10);

  if (ut_file_string_scanncomp (file, "***vox") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel file.\n");
    abort ();
  }

  if (ut_file_string_scanncomp (file, "**format") != 0
   || ut_file_string_scanncomp (file, "2.0") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  if (fscanf (file, "%s", *pformat) != 1
  || (! strcmp (*pformat, "ascii") && ! strncmp (*pformat, "bin", 3)))
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  if (ut_file_string_scanncomp (file, "**general") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  status = ut_array_1d_int_fscanf (file, (*pVox).size, 3);

  if (status != 1)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  status = ut_array_1d_fscanf (file, (*pVox).vsize, 3);

  if (status != 1)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  return;
}

/* Tessellation exportation: foot */
void
neut_vox_fscanf_foot (FILE * file)
{
  if (ut_file_string_scanncomp (file, "***end") != 0)
    abort ();

  return;
}

/* Tessellation exportation: vertex */
void
neut_vox_fscanf_data (struct VOX* pVox, char* format, FILE * file)
{
  int i, j, k;
  char c;
  FILE* file2 = NULL;
  char* filename = NULL;
  char* tmp = ut_alloc_1d_char (10);
  fpos_t pos;

  (*pVox).VoxPoly = ut_alloc_3d_int ((*pVox).size[0] + 2,
      (*pVox).size[1] + 2, (*pVox).size[2] + 2);

  if (fscanf (file, "%s", tmp) != 1)
    abort ();

  if (strcmp (tmp, "**voxel") != 0 && strcmp (tmp, "**data") != 0)
    abort ();

  do
  {
    fgetpos (file, &pos);
    if (fscanf (file, "%c", &c) != 1)
      abort ();
  }
  while (c == ' ' || c == '\n' || c == '\t');

  fsetpos (file, &pos);

  if (c == '*')
    if (fscanf (file, "%s", tmp) != 1)
      abort ();

  if (! strcmp (tmp, "*file"))
  {
    filename = ut_alloc_1d_char (1000);
    if (fscanf (file, "%s", filename) != 1)
      abort ();
    file2 = ut_file_open (filename, "r");
  }
  else
    file2 = file;

  if (! strcmp (format, "ascii"))
  {
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	  if (fscanf (file2, "%d", &((*pVox).VoxPoly[i][j][k])) != 1)
	    abort ();
  }

  else if (! strcmp (format, "binary8"))
  {
    unsigned char data;
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (unsigned char), 1, file2) != 1)
	   abort ();

	  (*pVox).VoxPoly[i][j][k] = data;
	}
  }

  else if (! strcmp (format, "binary16"))
  {
    short data;
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (short), 1, file2) != 1)
	    abort ();

	  (*pVox).VoxPoly[i][j][k] = (int) data;
	}
  }

  else if (! strcmp (format, "binary16*"))
  {
    short val2 = 0;
    char* pval = NULL;
    char* pval2 = (char*) &val2;

    short data;
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (short), 1, file2) != 1)
	    abort ();

	  pval = (char*) &data;
	  pval2[1] = pval[0];
	  pval2[0] = pval[1];

	  (*pVox).VoxPoly[i][j][k] = (int) val2;
	}
  }

  else if (! strcmp (format, "binary32")
	|| ! strcmp (format, "binary"  ))
  {
    int data;
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (int), 1, file2) != 1)
	    abort ();

	  (*pVox).VoxPoly[i][j][k] = data;
	}
  }

  else if (! strcmp (format, "binary32*"))
  {
    int val2 = 0;
    char* pval = NULL;
    char* pval2 = (char*) &val2;

    int data;
    for (k = 1; k <= (*pVox).size[2]; k++)
      for (j = 1; j <= (*pVox).size[1]; j++)
	for (i = 1; i <= (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (int), 1, file2) != 1)
	    abort ();

	  pval = (char*) &data;
	  pval2[3] = pval[0];
	  pval2[2] = pval[1];
	  pval2[1] = pval[2];
	  pval2[0] = pval[3];

	  (*pVox).VoxPoly[i][j][k] = (int) val2;
	}
  }


  else
    abort ();

  (*pVox).PolyQty = 0;
  for (k = 1; k <= (*pVox).size[2]; k++)
    for (j = 1; j <= (*pVox).size[1]; j++)
      for (i = 1; i <= (*pVox).size[0]; i++)
	(*pVox).PolyQty = ut_num_max_int ((*pVox).PolyQty, (*pVox).VoxPoly[i][j][k]);

  if (! strcmp (tmp, "*file"))
  {
    ut_file_close (file2, filename, "r");
    ut_free_1d_char (filename);
  }

  return;
}
