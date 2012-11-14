/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestPoly.h"

int
TestPoly (struct GEO Geo, int i, int verbosity)
{
  int res;

  if (verbosity)
    ut_print_message (0, 2, "Checking poly %d ...\n", i);

  res = TestPolyReciprocityFace (Geo, i, verbosity);
  if (res != 0)
    return res;

  res = TestPolyCoplaneityFace (Geo, i, verbosity);
  if (res != 0)
    return res;

  res = TestPolyOrientationFace (Geo, i, verbosity);
  if (res != 0)
    return res;

  return 0;
}
