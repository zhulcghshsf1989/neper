/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestVer.h"

int
TestVer (struct GEO Geo, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking vertex %d ...\n", i);

  if (TestVerEdgeQtyNNb      (Geo, i, verbosity) != 0
   || TestVerEdgeReciprocity (Geo, i, verbosity) != 0
   || TestVerBoundNCoo       (Geo, i, verbosity) != 0
   || TestVerFaceCoplaneity  (Geo, i, verbosity) != 0)
    return -1;
  else
    return 0;
}
