/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestEdge.h"

int
TestEdge (struct GEO Geo, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking edge %d ...\n", i);

  if (TestEdgeLength (Geo, i, verbosity) != 0
   || TestEdgeReciprocityVer (Geo, i, verbosity) != 0
   || TestEdgeReciprocityFace (Geo, i, verbosity) != 0)
    return -1;
  else
    return 0;
}
