/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"P2Pg.h"

/* P2Pg extracts polygon nb from the Poly polyhedron
 * description.
 */
void
P2Pg (struct POLY Poly, int nb, struct POLYGON *pPolygon)
{
  /* recording of the vertex quantity */
  printf ("1\n");
  P2PgVerQty (Poly, nb, pPolygon);
  /* recording of the polygon equation parameters */
  printf ("2\n");
  P2PgEq (Poly, nb, pPolygon);
  /* recording of the polygon vertex coordinates */
  printf ("3\n");
  P2PgVerCoo (Poly, nb, pPolygon);
  /* recording of the polygon center coordinates */
  printf ("4\n");
  P2PgCenterCoo (pPolygon);

  return;
}
