/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestFace.h"

int
TestFace (struct TESS Tess, int i, int verbosity)
{
  if (verbosity)
    ut_print_message (0, 2, "Checking face %d ...\n", i);

  if (TestFaceReciprocityEdge   (Tess, i, verbosity) != 0
   || TestFaceReciprocityVer    (Tess, i, verbosity) != 0
   || TestFaceReciprocityPoly   (Tess, i, verbosity) != 0
   || TestFaceState             (Tess, i, verbosity) != 0
   || TestFaceBound             (Tess, i, verbosity) != 0
   || TestFaceSelfIntersect     (Tess, i, verbosity) != 0
   || neut_tess_test_face_normal (Tess, i, verbosity) != 0)
   return -1;
  else
    return 0;
}
