/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestFace.h"

int
TestFace (struct GEO Geo, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking face %d ...\n", i);

  if (TestFaceReciprocityEdge   (Geo, i, verbosity) != 0
   || TestFaceReciprocityVer    (Geo, i, verbosity) != 0
   || TestFaceReciprocityPoly   (Geo, i, verbosity) != 0
   || TestFaceState             (Geo, i, verbosity) != 0
   || TestFaceBound             (Geo, i, verbosity) != 0
   || TestFaceSelfIntersect     (Geo, i, verbosity) != 0
   || neut_geo_test_face_normal (Geo, i, verbosity) != 0)
   return -1;
  else
    return 0;
}
