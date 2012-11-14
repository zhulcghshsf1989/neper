/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "TessDesUtils.h"

void
PolymodDescription (int num, struct POLYMOD c)
{
  int i, j;

  FILE *file;

  file = fopen ("PolymodDes", "a");

  fprintf (file, "\nPOLYMOD %d\n", num);

  fprintf (file, "VerQty=%d\n", c.VerQty);

  fprintf (file, "number\tUse\tFace\t\tCoo\n");
  for (i = 1; i <= c.VerQty; i++)
  {
    fprintf (file, "%d\t%d\t", i, c.VerUse[i]);
    for (j = 0; j <= 2; j++)
      fprintf (file, "%d ", c.VerFace[i][j]);
    fprintf (file, "  \t");
    for (j = 0; j <= 2; j++)
      fprintf (file, "%f\t", c.VerCoo[i][j]);
    fprintf (file, "\n");
  }

  fprintf (file, "\n");
  fprintf (file, "FaceQty=%d\n", c.FaceQty);
  fprintf (file, "number\tUse\tGerms\tPara\t\t\tVerQty\tVerNbs\n");
  for (i = 1; i <= c.FaceQty; i++)
  {
    fprintf (file, "%d\t%d\t%d\t%f\t%f\t%f\t%f\t%d\t\t", i, c.FaceUse[i],
	     c.FacePoly[i], c.FaceEq[i][0], c.FaceEq[i][1], c.FaceEq[i][2],
	     c.FaceEq[i][3], c.FaceVerQty[i]);
    for (j = 1; j <= c.FaceVerQty[i]; j++)
      fprintf (file, "%d ", c.FaceVerNb[i][j]);
    fprintf (file, "\n");
  }

  fclose (file);

  return;
}


void
PolyDescription (int num, POLY c)
{
  int i, j;

  FILE *P;

  P = fopen ("PolyDes", "a");

  fprintf (P, "\n\nPOLY %d description\n", num);

  fprintf (P, "VerQty=%d\n", c.VerQty);

  fprintf (P, "number\tFace\t\t\tCoo\n");
  for (i = 1; i <= c.VerQty; i++)
  {
    fprintf (P, "%d\t", i);
    for (j = 0; j <= 2; j++)
      fprintf (P, "%d\t", c.VerFace[i][j]);
    for (j = 0; j <= 2; j++)
      fprintf (P, "%f\t", c.VerCoo[i][j]);
    fprintf (P, "\n");
  }

  fprintf (P, "\nFaceQty=%d\n", c.FaceQty);
  fprintf (P, "number\tGerms\tPara\t\t\tVerQty\tVerNbs\n");
  for (i = 1; i <= c.FaceQty; i++)
  {
    fprintf (P, "%d\t%d\t%f\t%f\t%f\t%f\t%d\t", i, c.FacePoly[i],
	     c.FaceEq[i][0], c.FaceEq[i][1], c.FaceEq[i][2], c.FaceEq[i][3],
	     c.FaceVerQty[i]);
    for (j = 1; j <= c.FaceVerQty[i]; j++)
      fprintf (P, "%d ", c.FaceVerNb[i][j]);
    fprintf (P, "\n");
  }

  fprintf (P, "\n\n\n");

  fclose (P);

  return;
}

void
TessDes (struct TESS *pTess)
{
  int i, j, k;
  FILE *file;

  file = fopen ("TessDes", "w");

  fprintf (file, "(*pTess).VerQty=%d\n", (*pTess).VerQty);
  fprintf (file, "(*pTess).FaceQty=%d\n", (*pTess).FaceQty);
  fprintf (file, "(*pTess).EdgeQty=%d\n", (*pTess).EdgeQty);
  fprintf (file, "(*pTess).PolyQty=%d\n", (*pTess).PolyQty);

  for (i = 1; i <= (*pTess).VerQty; i++)
  {
    for (k = 0; k <= 3; k++)
      fprintf (file, "(*pTess).VerGerm[%d][%d]=%d\n", i, k,
	       (*pTess).VerGerm[i][k]);
    for (k = 0; k <= 2; k++)
      fprintf (file, "(*pTess).VerCoo[%d][%d]=%f\n", i, k,
	       (*pTess).VerCoo[i][k]);
  }

  for (i = 1; i <= (*pTess).EdgeQty; i++)
    fprintf (file, "edge %d: %d & %d\n", i, (*pTess).EdgeVerNb[i][0],
	     (*pTess).EdgeVerNb[i][1]);

  for (i = 1; i <= (*pTess).FaceQty; i++)
  {
    for (k = 0; k <= 1; k++)
      fprintf (file, "(*pTess).FacePoly[%d][%d]=%d\n", i, k,
	       (*pTess).FacePoly[i][k]);

    for (k = 0; k <= 3; k++)
      fprintf (file, "(*pTess).FaceEq[%d][%d]=%f\n", i, k,
	       (*pTess).FaceEq[i][k]);

    fprintf (file, "(*pTess).FaceVerQty[%d]=%d\n", i, (*pTess).FaceVerQty[i]);
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      fprintf (file, "(*pTess).FaceVerNb[%d][%d]=%d\n", i, j,
	       (*pTess).FaceVerNb[i][j]);
    for (j = 1; j <= (*pTess).FaceVerQty[i]; j++)
      fprintf (file, "(*pTess).FaceEdgeNb[%d][%d]=%d\n", i, j,
	       (*pTess).FaceEdgeNb[i][j]);
  }

  for (i = 1; i <= (*pTess).PolyQty; i++)
  {
    for (k = 0; k <= 2; k++)
      fprintf (file, "(*pTess).CenterCoo[%d][%d]=%f\n", i, k,
	       (*pTess).CenterCoo[i][k]);

    fprintf (file, "(*pTess).PolyVerQty[%d]=%d\n", i, (*pTess).PolyVerQty[i]);
    for (j = 1; j <= (*pTess).PolyVerQty[i]; j++)
      fprintf (file, "(*pTess).PolyVerNb[%d][%d]=%d\n", i, j,
	       (*pTess).PolyVerNb[i][j]);

    fprintf (file, "(*pTess).PolyFaceQty[%d]=%d\n", i,
	     (*pTess).PolyFaceQty[i]);
    for (j = 1; j <= (*pTess).PolyFaceQty[i]; j++)
      fprintf (file, "(*pTess).PolyFaceNb[%d][%d]=%d\n", i, j,
	       (*pTess).PolyFaceNb[i][j]);

    fprintf (file, "(*pTess).PolyEdgeQty[%d]=%d\n", i,
	     (*pTess).PolyEdgeQty[i]);
    for (j = 1; j <= (*pTess).PolyEdgeQty[i]; j++)
      fprintf (file, "(*pTess).PolyEdgeNb[%d][%d]=%d\n", i, j,
	       (*pTess).PolyEdgeNb[i][j]);
  }

  fclose (file);

  return;
}
