/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_tess_deform.h"

/* Deformation of the tessellation.
 */
void
neut_tess_deform (struct TESS *pTess, double *gsize)
{
  int i, j = 0, numa, numb;
  double norm;

  /* TODO alloc should probably not be done */
  (*pTess).EdgeLength = ut_alloc_1d ((*pTess).EdgeQty + 1);

  /* changing vertex coordinates */
  for (i = 1; i <= (*pTess).VerQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).VerCoo[i][j] *= gsize[j];

  /* updating edge lengths from the ver positions */
  for (i = 1; i <= (*pTess).EdgeQty; i++)
  {
    numa = (*pTess).EdgeVerNb[i][0];
    numb = (*pTess).EdgeVerNb[i][1];

    (*pTess).EdgeLength[i] = ut_space_dist ((*pTess).VerCoo[numa], (*pTess).VerCoo[numb]);
  }

  /* updating face equations */
  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    (*pTess).FaceEq[i][1] /= gsize[0];
    (*pTess).FaceEq[i][2] /= gsize[1];
    (*pTess).FaceEq[i][3] /= gsize[2];
    norm = ut_vector_norm ((*pTess).FaceEq[i] + 1);
    for (j = 0; j < 4; j++)
      (*pTess).FaceEq[i][j] /= norm;
  }

  /* changing centre coordinates */
  for (i = 1; i <= (*pTess).PolyQty; i++)
    for (j = 0; j < 3; j++)
      (*pTess).CenterCoo[i][j] *= gsize[j];

  return;
}
