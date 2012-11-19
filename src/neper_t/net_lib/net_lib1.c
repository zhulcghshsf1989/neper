/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_lib.h"

void
net_lib_geo_import (char* libpath, char* morpho, int n, struct GEO* pGeo)
{
  char* name = NULL;
  FILE* file = NULL;

  if (libpath == NULL)
  {
    ut_print_message (2, 0, "libpath not initialized.\n");
    abort ();
  }

  name = ut_alloc_1d_char (strlen (libpath) + 100);

  sprintf (name, "%s/%s/%s%d.tess", libpath, morpho, morpho, n);

  if (ut_file_exist (name) == 1)
  {
    file = ut_file_open (name, "R");
    neut_geo_fscanf (file, pGeo);
    ut_file_close (file, name, "R");
  }
  else
    ut_print_message (2, 0, "File %s does not exist.\n", name);

  neut_geo_init_polybody (pGeo);
  neut_geo_init_polytrue (pGeo);

  ut_free_1d_char (name);

  return;
}
