/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"P2Pg.h"

/* recording of the vertex quantity */
void
P2PgVerQty (struct POLY Poly, int nb, struct POLYGON *pPolygon)
{
  (*pPolygon).VerQty = Poly.FaceVerQty[nb];

  return;
}

/* recording of the polygon equation parameters */
void
P2PgEq (struct POLY Poly, int nb, struct POLYGON *pPolygon)
{
  int i;

  (*pPolygon).Eq = oneDDoubleDAlloc (4);

  for (i = 0; i <= 3; i++)
    (*pPolygon).Eq[i] = Poly.FaceEq[nb][i];

  return;
}

/* recording of the polygon vertex coordinates */
void
P2PgVerCoo (struct POLY Poly, int nb, struct POLYGON *pPolygon)
{
  int i, j;
  int PV;			/* polyhedron vertex number */

  (*pPolygon).VerCoo = twoDDoubleDAlloc ((*pPolygon).VerQty + 1, 3);

  for (i = 1; i <= (*pPolygon).VerQty; i++)
  {
    PV = Poly.FaceVerNb[nb][i];
    for (j = 0; j <= 2; j++)
      (*pPolygon).VerCoo[i][j] = Poly.VerCoo[PV][j];
  }

  return;
}

/* recording of the polygon center coordinates */
void
P2PgCenterCoo (struct POLYGON *pPolygon)
{
  int i, j;
  double tmp;

  (*pPolygon).CenterCoo = oneDDoubleDAlloc (3);

  for (j = 0; j <= 2; j++)
  {
    tmp = 0;
    for (i = 1; i <= (*pPolygon).VerQty; i++)
      tmp += (*pPolygon).VerCoo[i][j];

    (*pPolygon).CenterCoo[j] = tmp / (double) (*pPolygon).VerQty;
  }

  return;
}
