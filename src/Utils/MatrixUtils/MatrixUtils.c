/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "MatrixUtils.h"

double
MatrixDeterminant (int size, double **matrix)
{
  int i, j, k, l, ii, jj;
  double res = 0;
  double **temp;
  double det;
  temp = twoDDoubleDAlloc (size, size);

  if (size == 1)
    res = matrix[1][1];
  else
    for (i = 1; i <= 1; i++)
      for (j = 1; j <= size; j++)
      {
	ii = 0;
	for (k = 1; k <= size; k++)
	  if (k != i)
	  {
	    ii++;
	    jj = 0;
	    for (l = 1; l <= size; l++)
	      if (l != j)
	      {
		jj++;
		temp[ii][jj] = matrix[k][l];
	      }
	  }
	det = MatrixDeterminant (size - 1, temp);
	res += pow (-1, 1 + j) * matrix[1][j] * det;
      }

  twoDDoubleFree (temp, size);

  return res;
}

void
Comatrix (int size, double **matrix, double **comatrix)
{
  int i, j, k, l, ii, jj;
  double **temp;
  double det;
  temp = twoDDoubleDAlloc (size, size);

  if (size == 2)
  {
    comatrix[1][1] = matrix[2][2];
    comatrix[2][2] = matrix[1][1];
    comatrix[1][2] = -matrix[2][1];
    comatrix[2][1] = -matrix[1][2];
  }

  else
  {
    for (i = 1; i <= size; i++)
      for (j = 1; j <= size; j++)
      {
	ii = 0;
	for (k = 1; k <= size; k++)
	  if (k != i)
	  {
	    ii++;
	    jj = 0;
	    for (l = 1; l <= size; l++)
	      if (l != j)
	      {
		jj++;
		temp[ii][jj] = matrix[k][l];
	      }
	  }
	det = MatrixDeterminant (size - 1, temp);
	comatrix[i][j] = pow (-1, i + j) * det;;
      }
  }

  twoDDoubleFree (temp, size);

  return;
}


void
TransposeMatrix (int size, double **matrix, double **tmatrix)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 1; j <= size; j++)
      tmatrix[i][j] = matrix[j][i];

  return;
}

void
MultiplyMatrix (int size, double **matrix, double number, double **resmatrix)
{
  int i, j;

  for (i = 1; i <= size; i++)
    for (j = 1; j <= size; j++)
      resmatrix[i][j] = number * matrix[i][j];

  return;
}


void
InverseMatrix (int size, double **matrix, double **invmatrix)
{
  double det;
  double **comatrix;
  double **transmatrix;
  comatrix = twoDDoubleDAlloc (size + 1, size + 1);
  transmatrix = twoDDoubleDAlloc (size + 1, size + 1);

  det = MatrixDeterminant (size, matrix);
  Comatrix (size, matrix, comatrix);
  TransposeMatrix (size, comatrix, transmatrix);
  MultiplyMatrix (size, transmatrix, 1 / det, invmatrix);

  twoDDoubleFree (comatrix, size + 1);
  twoDDoubleFree (transmatrix, size + 1);

  return;
}

void
MatrixByVector (int size, double **invA, double *B, double *X)
{
  int i, j;
  double value;

  for (i = 1; i <= size; i++)
  {
    value = 0;
    for (j = 1; j <= size; j++)
      value += invA[i][j] * B[j];
    X[i] = value;
  }

  return;
}

void
SolveMatrix (int size, double **A, double *X, double *B)
{
  double **invA;
  invA = (double **) twoDDoubleDAlloc (size + 1, size + 1);

  InverseMatrix (size, A, invA);

  MatrixByVector (size, invA, B, X);

  twoDDoubleFree (invA, size + 1);

  return;
}
