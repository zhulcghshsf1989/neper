/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_mtess_topo_lcl.h"

int
neut_mtess_tess_level_dom (struct MTESS MTess, struct TESS Tess, int
    level, int* dom)
{
  int i;

  if (Tess.TessId <= 0)
    ut_error_reportbug ();

  if (level >= Tess.Level)
    return -1;

  dom[0] = Tess.TessId;
  for (i = Tess.Level - 1; i >= level; i--)
    ut_array_1d_int_memcpy (dom, 2, MTess.TessDom[dom[0]]);

  return 0;
}
