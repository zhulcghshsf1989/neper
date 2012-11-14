/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "GslUtils.h"

int
PoissonNb (double mu, int seed, int id)
{
  int i;
  int res = 0;

  gsl_rng *r;

  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, seed);

  for (i = 1; i <= id; i++)
    res = gsl_ran_poisson (r, mu);

  gsl_rng_free (r);

  return res;
}
