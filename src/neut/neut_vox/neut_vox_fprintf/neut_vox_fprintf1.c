/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_vox_fprintf_lcl.h"

void
neut_vox_fprintf (FILE* file, char* format, struct VOX Vox)
{
  char* format2 = ut_alloc_1d_char (100);

  if (format == NULL || strlen (format) == 0 || ! strcmp (format, "default"))
  {
    if (Vox.PolyQty < 256)
      strcpy (format2, "binary8");
    else if (Vox.PolyQty < 65536)
      strcpy (format2, "binary16");
    else
      strcpy (format2, "binary32");
  }
  else
    strcpy (format2, format);

  neut_vox_fprintf_head (file, format2, Vox);

  neut_vox_fprintf_data (file, format2, Vox);
  neut_vox_fprintf_foot (file);

  ut_free_1d_char (format2);

  return;
}

void
neut_vox_name_fprintf_raw (char* vox, char* raw, char* format, struct VOX Vox)
{
  char* format2 = ut_alloc_1d_char (100);
  FILE* fp1 = NULL;

  fp1 = ut_file_open (vox, "w");

  if (format == NULL || strlen (format) == 0 || ! strcmp (format, "default"))
  {
    if (Vox.PolyQty < 256)
      strcpy (format2, "binary8");
    else if (Vox.PolyQty < 65536)
      strcpy (format2, "binary16");
    else
      strcpy (format2, "binary32");
  }
  else
    strcpy (format2, format);

  neut_vox_fprintf_head (fp1, format2, Vox);
  neut_vox_fprintf_data_raw (fp1, raw, format2, Vox);
  neut_vox_fprintf_foot (fp1);

  ut_file_close (fp1, vox, "w");

  ut_free_1d_char (format2);

  return;
}
