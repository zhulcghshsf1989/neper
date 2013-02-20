/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_rand.h"

int
neut_rand_nnid2rand (int N, int id, int level, int poly)
{
  time_t t;
  int Rand;

  /* Rand: long integer number calculated from (N,id,level,poly):
   * Rand is 8 numbers long ; N is put at the left side, id at the
   * right side. If N and id numbers cross, they are added.
   * N=123  and id=12     --> Rand=12300012
   * N=3000 and id=65433  --> Rand=30065433
   * N=3333 and id=88888  --> Rand=33418888
   * To this value is added the product level * poly.  It is put at the
   * left hand side, but with a length of 7.
   */

  if (id >= 0)
    Rand = N * pow (10, 7 - ut_num_tenlen (N))
         + poly * level * pow (10, 6 - ut_num_tenlen (poly * level))
	 + id;
  else
  {
    time (&t);
    Rand = t;
  }

  return Rand;
}

int
neut_rand_poisson (double mu, int seed, int id)
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
