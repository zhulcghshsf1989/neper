/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceFF.h"

double
UpdateFaceFF (struct TESS *pTess, int ver)
{
  return FaceTessFF (pTess, ver);
}
