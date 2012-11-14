/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FaceFF.h"

double
FaceGeoFF (struct GEO *pGeo, int ver)
{
  int i, faceqty;
  double res;
  int* face = NULL;
  double* FFF = NULL;

  neut_geo_ver_faces (*pGeo, ver, &face, &faceqty);

  FFF = ut_alloc_1d (faceqty);
  for (i = 0; i < faceqty; i++)
    FFF[i] = FFBaryFace (pGeo, face[i]);

  res = ut_array_1d_max (FFF, faceqty);

  ut_free_1d_int (face);
  ut_free_1d (FFF);

  return res;
}
