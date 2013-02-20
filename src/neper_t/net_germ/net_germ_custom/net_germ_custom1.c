/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ_custom.h"

void
net_germ_custom (char* filename, struct POLY Dom, struct GERMSET* pGSet)
{
  FILE* file = NULL;

  (*pGSet).Size = ut_alloc_1d (7);
  neut_poly_bbox (Dom, (*pGSet).Size);

  (*pGSet).GermCoo = ut_alloc_2d ((*pGSet).N + 1, 3);

  file = ut_file_open (filename, "r");
  ut_array_2d_fscanf (file, (*pGSet).GermCoo + 1, (*pGSet).N, 3);
  ut_file_close (file, filename, "r");

  return;
}

