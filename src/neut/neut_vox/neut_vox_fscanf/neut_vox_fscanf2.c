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
   || ut_file_string_scanncomp (file, "1.10") != 0)
  {
    ut_print_message (2, 0, "Input file is not a valid voxel tessellation file.\n");
    abort ();
  }

  if (fscanf (file, "%s", *pformat) != 1
  || (! strcmp (*pformat, "ascii") && ! strncmp (*pformat, "binary", 6)))
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

  if (ut_file_string_scanncomp (file, "**voxel") != 0)
    abort ();

  (*pVox).VoxPoly = ut_alloc_3d_int ((*pVox).size[0], (*pVox).size[1], (*pVox).size[2]);

  if (! strcmp (format, "ascii"))
    for (k = 0; k < (*pVox).size[2]; k++)
      for (j = 0; j < (*pVox).size[1]; j++)
	for (i = 0; i < (*pVox).size[0]; i++)
	{
	  if (fscanf (file, "%d", &((*pVox).VoxPoly[i][j][k])) != 1)
	    abort ();
	}

  else if (! strcmp (format, "binary8"))
  {
    if (fscanf (file, "%c", &c) != 1) // get rid of the '\n' right after the format string
     abort ();

    unsigned char data;
    for (k = 0; k < (*pVox).size[2]; k++)
      for (j = 0; j < (*pVox).size[1]; j++)
	for (i = 0; i < (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (unsigned char), 1, file) != 1)
	   abort ();

	  (*pVox).VoxPoly[i][j][k] = data;
	}
  }

  else if (! strcmp (format, "binary16"))
  {
    if (fscanf (file, "%c", &c) != 1) // idem
      abort ();

    short data;
    for (k = 0; k < (*pVox).size[2]; k++)
      for (j = 0; j < (*pVox).size[1]; j++)
	for (i = 0; i < (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (short), 1, file) != 1)
	    abort ();

	  (*pVox).VoxPoly[i][j][k] = data;
	}
  }

  else if (! strcmp (format, "binary32")
	|| ! strcmp (format, "binary"  ))
  {
    if (fscanf (file, "%c", &c) != 1) // idem
      abort ();

    int data;
    for (k = 0; k < (*pVox).size[2]; k++)
      for (j = 0; j < (*pVox).size[1]; j++)
	for (i = 0; i < (*pVox).size[0]; i++)
	{
	  if (fread (&data, sizeof (int), 1, file) != 1)
	    abort ();

	  (*pVox).VoxPoly[i][j][k] = data;
	}
  }

  else
    abort ();

  (*pVox).PolyQty = 0;
  for (k = 0; k < (*pVox).size[2]; k++)
    for (j = 0; j < (*pVox).size[1]; j++)
      for (i = 0; i < (*pVox).size[0]; i++)
	(*pVox).PolyQty = ut_num_max_int ((*pVox).PolyQty, (*pVox).VoxPoly[i][j][k]);

  return;
}
