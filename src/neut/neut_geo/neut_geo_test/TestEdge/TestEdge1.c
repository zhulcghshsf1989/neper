/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestEdge.h"

int
TestEdge (struct GEO Geo, int i)
{
  int res;

  res = TestEdgeLength (Geo, i);
  if (res != 0)
    return res;
  res = TestEdgeReciprocityVer (Geo, i);
  if (res != 0)
    return res;
  res = TestEdgeReciprocityFace (Geo, i);
  if (res != 0)
    return res;

  return 0;
}
