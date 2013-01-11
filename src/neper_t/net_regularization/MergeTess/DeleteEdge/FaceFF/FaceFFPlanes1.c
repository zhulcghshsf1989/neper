/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceFF.h"

double
FaceTessFF (struct TESS *pTess, int ver)
{
  int i, faceqty;
  double res;
  int* face = NULL;
  double* FFF = NULL;

  neut_tess_ver_faces (*pTess, ver, &face, &faceqty);

  FFF = ut_alloc_1d (faceqty);
  for (i = 0; i < faceqty; i++)
    FFF[i] = FFBaryFace (pTess, face[i]);

  res = ut_array_1d_max (FFF, faceqty);

  ut_free_1d_int (face);
  ut_free_1d (FFF);

  return res;
}
