/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "FreeStruct.h"

void
PolyFree (struct POLY *Poly, int N)
{
  int i;

  for (i = 1; i <= N; i++)
    neut_poly_free (&(Poly[i]));

  free (Poly);

  return;
}
