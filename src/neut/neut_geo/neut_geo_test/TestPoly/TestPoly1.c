/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestPoly.h"

int
TestPoly (struct GEO Geo, int i)
{
  int res;

  res = TestPolyReciprocityFace (Geo, i);
  if (res != 0)
  {
    printf ("TestPolyReciprocityFace failed\n");
    return res;
  }
  res = TestPolyCoplaneityFace (Geo, i);
  if (res != 0)
  {
    ut_print_message (1, 0, "Poly %d has coplanar faces.  This may be a bug.\n", i);
    return 0;
    // return res;
  }
  res = TestPolyOrientationFace (Geo, i);
  if (res != 0)
  {
    printf ("TestPolyOrientationFace failed\n");
    return res;
  }

  return 0;
}
