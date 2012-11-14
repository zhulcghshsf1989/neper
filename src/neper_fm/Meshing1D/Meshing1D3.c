/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"Meshing1D.h"

double
NextCoo (double coo, double a, double b)
{
  double newcoo;

  if (a >= 0)
    newcoo = coo + a * coo + b;
  else
    newcoo = (coo + b) / (1 - a);

  return newcoo;
}
