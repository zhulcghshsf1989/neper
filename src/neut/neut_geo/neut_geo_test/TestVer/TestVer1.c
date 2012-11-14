/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestVer.h"

int
TestVer (struct GEO Geo, int i, int print)
{
  int res;

  res = TestVerEdgeQtyNNb (Geo, i, print);
  if (res != 0)
    return res;
  res = TestVerEdgeReciprocity (Geo, i);
  if (res != 0)
    return res;
  res = TestVerBoundNCoo (Geo, i);
  if (res != 0)
    return res;
  res = TestVerFaceCoplaneity (Geo, i);
  if (res != 0)
    return res;

  return 0;
}
