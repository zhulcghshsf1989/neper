/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestVer.h"

int
TestVer (struct TESS Tess, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking vertex %d ...\n", i);

  if (TestVerEdgeQtyNNb      (Tess, i, verbosity) != 0
   || TestVerEdgeReciprocity (Tess, i, verbosity) != 0
   || TestVerBoundNCoo       (Tess, i, verbosity) != 0
   || TestVerFaceCoplaneity  (Tess, i, verbosity) != 0)
    return -1;
  else
    return 0;
}
