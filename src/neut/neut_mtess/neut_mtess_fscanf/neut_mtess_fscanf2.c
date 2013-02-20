/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_fscanf_lcl.h"

int
neut_mtess_fscanf_head (struct MTESS* pMTess, FILE* file)
{
  int i;
  char* string = ut_alloc_1d_char (100);

  if (fscanf (file, "%s", string) != 1)
  {
    ut_print_message (2, 0, "Input file is not a valid multilevel tessellation file.\n");
    abort ();
  }

  if (! strcmp (string, "***mtess"))
  {
    if (ut_file_string_scanncomp (file, "**format") != 0
     || ut_file_string_scanncomp (file, "2.0")      != 0
     || ut_file_string_scanncomp (file, "**general") != 0)
    {
      ut_print_message (2, 0, "Input file is not a valid multilevel tessellation file.\n");
      abort ();
    }

    if (fscanf (file, "%d", &((*pMTess).LevelQty)) != 1)
    {
      ut_print_message (2, 0, "Input file is not a valid multilevel tessellation file.\n");
      abort ();
    }

    (*pMTess).LevelTessQty = ut_alloc_1d_int ((*pMTess).LevelQty + 1);
    (*pMTess).LevelTessQty[0] = 1;

    (*pMTess).LevelTess = ut_alloc_1d_pint ((*pMTess).LevelQty + 1);
    for (i = 1; i <= (*pMTess).LevelQty; i++)
    {
      if (fscanf (file, "%d", &((*pMTess).LevelTessQty[i])) != 1)
	abort ();

      (*pMTess).LevelTess[i] = ut_alloc_1d_int ((*pMTess).LevelTessQty[i] + 1);
      ut_array_1d_int_fscanf (file, (*pMTess).LevelTess[i] + 1, (*pMTess).LevelTessQty[i]);
    }

    if (fscanf (file, "%d", &((*pMTess).TessQty)) != 1)
      abort ();

    (*pMTess).TessDom = ut_alloc_2d_int ((*pMTess).TessQty + 1, 2);
    ut_array_2d_int_fscanf (file, (*pMTess).TessDom + 1, (*pMTess).TessQty, 2);

    if (ut_file_string_scanncomp (file, "***end") != 0)
      abort ();

    return 0;
  }

  else if (! strcmp (string, "***tess"))
  {
    fseek (file, 0, 0);
    (*pMTess).LevelQty = 1;

    (*pMTess).LevelTessQty = ut_alloc_1d_int ((*pMTess).LevelQty + 1);
    (*pMTess).LevelTessQty[0] = 1;

    (*pMTess).LevelTess = ut_alloc_1d_pint ((*pMTess).LevelQty + 1);

    (*pMTess).LevelTessQty[1] = 1;
    (*pMTess).LevelTess[1] = ut_alloc_1d_int ((*pMTess).LevelTessQty[1] + 1);
    (*pMTess).LevelTess[1][1] = 1;

    (*pMTess).TessQty = 1;
    (*pMTess).TessDom = ut_alloc_2d_int ((*pMTess).TessQty + 1, 2);
    (*pMTess).TessDom[1][1] = 1;

    return 1;
  }

  else
  {
    ut_print_message (2, 0, "Input file is not a valid multilevel tessellation file.\n");
    abort ();
  }
}

