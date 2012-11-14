/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "FreeUtils.h"

void
oneDIntFree (int *res)
{
  free (res);

  return;
}

void
oneDFloatFree (float *res)
{
  free (res);

  return;
}

void
oneDDoubleFree (double *res)
{
  free (res);

  return;
}

void
twoDIntFree (int **array, unsigned int line)
{
  int i;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);

  return;
}

void
twoDFloatFree (float **array, unsigned int line)
{
  int i;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);

  return;
}

void
twoDDoubleFree (double **array, unsigned int line)
{
  int i;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);

  return;
}

void
threeDIntFree (int ***array, unsigned int line, unsigned int column)
{
  int i, j;

  for (i = line - 1; i >= 0; i--)
    for (j = column - 1; j >= 0; j--)
      free (array[i][j]);

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);

  return;
}


void
oneDCharFree (char *string)
{
  free (string);

  return;
}

void
twoDCharFree (char **array, unsigned int line)
{
  int i;

  for (i = line - 1; i >= 0; i--)
    free (array[i]);

  free (array);

  return;
}

extern void
oneDpFileFree (FILE ** files)
{
  free (files);

  return;
}
