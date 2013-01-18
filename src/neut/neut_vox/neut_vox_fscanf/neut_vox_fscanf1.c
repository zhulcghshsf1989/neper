/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_vox_fscanf_lcl.h"

void
neut_vox_fscanf (FILE* file, struct VOX* pVox)
{
  char* format = NULL;

  neut_vox_fscanf_head (pVox, &format, file);
  neut_vox_fscanf_data  (pVox, format, file);
  neut_vox_fscanf_foot (file);

  ut_free_1d_char (format);

  return;
}

void
neut_vox_name_fscanf (char* name, struct VOX* pVox)
{
  FILE* file = ut_file_open (name, "r");
  neut_vox_fscanf (file, pVox);
  ut_file_close (file, name, "r");

  return;
}
