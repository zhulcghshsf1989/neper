/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "AllocUtils.h"

float *
oneDFloatDAlloc (unsigned int line)
{
  float *vector;

  vector = (float *) calloc (line, sizeof (float));

  return vector;
}

double *
oneDDoubleDAlloc (unsigned int line)
{
  double *vector;

  vector = (double *) calloc (line, sizeof (double));

  return vector;
}

char *
oneDCharDAlloc (unsigned int line)
{
  char *res;

  res = (char *) calloc (line, sizeof (char));

  return res;
}

FILE **
oneDpFileDAlloc (unsigned int line)
{
  FILE **vector;

  vector = (FILE **) calloc (line, sizeof (FILE *));

  return vector;
}

int **
twoDIntDAlloc (unsigned int line, unsigned int column)
{
  unsigned int i;
  int **array;

  array = (int **) calloc (line, sizeof (int *));

  for (i = 0; i <= line - 1; i++)
    array[i] = (int *) calloc (column, sizeof (int));

  return array;
}

float **
twoDFloatDAlloc (unsigned int line, unsigned int column)
{
  unsigned int i;
  float **array;

  array = (float **) calloc (line, sizeof (float *));

  for (i = 0; i <= line - 1; i++)
    array[i] = (float *) calloc (column, sizeof (float));

  return array;
}

double **
twoDDoubleDAlloc (unsigned int line, unsigned int column)
{
  unsigned int i;
  double **array;

  array = (double **) calloc (line, sizeof (double *));

  for (i = 0; i <= line - 1; i++)
    array[i] = (double *) calloc (column, sizeof (double));

  return array;
}

char **
twoDCharDAlloc (unsigned int line, unsigned int column)
{
  unsigned int i;
  char **array;

  array = (char **) calloc (line, sizeof (char *));

  for (i = 0; i <= line - 1; i++)
    array[i] = (char *) calloc (column, sizeof (char));

  return array;
}

int ***
threeDIntDAlloc (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  int ***array;

  array = (int ***) calloc (line, sizeof (int **));

  for (i = 0; i <= line - 1; i++)
  {
    array[i] = (int **) calloc (column, sizeof (int *));
    for (j = 0; j <= column - 1; j++)
      array[i][j] = (int *) calloc (depth, sizeof (int));
  }

  return array;
}

double ***
threeDDoubleDAlloc (unsigned int line, unsigned int column,
		    unsigned int depth)
{
  unsigned int i, j;
  double ***array;

  array = (double ***) calloc (line, sizeof (double **));

  for (i = 0; i <= line - 1; i++)
  {
    array[i] = (double **) calloc (column, sizeof (double *));
    for (j = 0; j <= column - 1; j++)
      array[i][j] = (double *) calloc (depth, sizeof (double));
  }

  return array;
}

char ***
threeDCharDAlloc (unsigned int line, unsigned int column, unsigned int depth)
{
  unsigned int i, j;
  char ***array;

  array = (char ***) calloc (line, sizeof (char **));

  for (i = 0; i <= line - 1; i++)
  {
    array[i] = (char **) calloc (column, sizeof (char *));
    for (j = 0; j <= column - 1; j++)
      array[i][j] = (char *) calloc (depth, sizeof (char));
  }

  return array;
}

int **
oneDpIntDAlloc (unsigned int line)
{
  int **vector;

  vector = (int **) calloc (line, sizeof (int *));

  return vector;
}

double **
oneDpDoubleDAlloc (unsigned int line)
{
  double **vector;

  vector = (double **) calloc (line, sizeof (double *));

  return vector;
}

int ***
oneDppIntDAlloc (unsigned int line)
{
  int ***vector;

  vector = (int ***) calloc (line, sizeof (int **));

  return vector;
}

double ***
oneDppDoubleDAlloc (unsigned int line)
{
  double ***vector;

  vector = (double ***) calloc (line, sizeof (double **));

  return vector;
}
