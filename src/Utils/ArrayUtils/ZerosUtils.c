/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"ZerosUtils.h"

void
oneDIntZeros (int *array, int beg, int end)
{
  int i;

  for (i = beg; i <= end; i++)
    array[i] = 0;

  return;
}

void
oneDFloatZeros (float *array, int beg, int end)
{
  int i;

  for (i = beg; i <= end; i++)
    array[i] = 0;

  return;
}

void
oneDDoubleZeros (double *array, int beg, int end)
{
  int i;

  for (i = beg; i <= end; i++)
    array[i] = 0;

  return;
}

void
twoDIntZeros (int **array, int begX, int endX, int begY, int endY)
{
  int i, j;

  for (i = begX; i <= endX; i++)
    for (j = begY; i <= endY; i++)
      array[i][j] = 0;

  return;
}

void
twoDFloatZeros (float **array, int begX, int endX, int begY, int endY)
{
  int i, j;

  for (i = begX; i <= endX; i++)
    for (j = begY; i <= endY; i++)
      array[i][j] = 0;

  return;
}

void
twoDDoubleZeros (double **array, int begX, int endX, int begY, int endY)
{
  int i, j;

  for (i = begX; i <= endX; i++)
    for (j = begY; i <= endY; i++)
      array[i][j] = 0;

  return;
}
