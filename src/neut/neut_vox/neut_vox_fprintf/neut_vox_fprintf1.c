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
