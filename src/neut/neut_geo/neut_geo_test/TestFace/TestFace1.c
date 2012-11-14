/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestFace.h"

int
TestFace (struct GEO Geo, int i)
{
  int res;
  int verbosity = 0;

  res = TestFaceReciprocityEdge (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceReciprocityEdge: %d\n", res);
  if (res != 0)
    return res;

  res = TestFaceReciprocityVer (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceReciprocityVer: %d\n", res);
  if (res != 0)
    return res;

  res = TestFaceReciprocityPoly (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceReciprocityPoly: %d\n", res);
  if (res != 0)
    return res;

  res = TestFaceState (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceState: %d\n", res);
  if (res != 0)
    return res;

  res = TestFaceBound (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceBound: %d\n", res);
  if (res != 0)
    return res;

  res = TestFaceSelfIntersect (Geo, i);
  if (verbosity == 1)
    printf ("TestFaceSelfIntersect: %d\n", res);
  if (res != 0)
    return res;

  res = neut_geo_test_face_normal (Geo, i);
  if (verbosity == 1)
    printf ("neut_geo_test_face_normal: %d\n", res);
  if (res != 0)
    return res;

  return 0;
}
