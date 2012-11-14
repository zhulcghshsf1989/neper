/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTPOLYGON_H
#define STRUCTPOLYGON_H

struct POLYGON
{
  /* GENERAL INFORMATION ***************************************************
   */
  int VerQty;			/* Polygon quantities of vertex */

  /* POLYGON INFORMATION ***************************************************
   */
  double *Eq;			/* Eq[i][j] (j=0...3) are the four equation */
  /* parameters: */
  /* Eq[i][1]*X1+Eq[i][2]*X2+Eq[i][3]*X3=Eq[i][0] */
  double Area;			/* Area of the polygon */

  /* VERTEX INFORMATION ****************************************************
   */
  /* For vertex i (i=1...VerQty): */
  double **VerCoo;		/* VerCoo[i][j] (j=0...2) are the 3 coordinates. */
  double *CenterCoo;		/* CenterCoo[j] (j=0...2) are the 3 coordinates. */

};
typedef struct POLYGON POLYGON;

#endif /* STRUCTPOLYGON_H */
