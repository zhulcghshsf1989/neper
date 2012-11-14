/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "RMeshing.h"

void
nemm_in_msize (struct IN In, double** domainsize, int* msize)
{
  int i;
  double* dsize = ut_alloc_1d (3);
  double avdsize;

  if (In.msizetype == 1)
  {
    for (i = 0; i < 3; i++)
      dsize[i] = domainsize[i][1] - domainsize[i][0];
    avdsize = pow (dsize[0] * dsize[1] * dsize[2], 0.3333333333);

    for (i = 0; i < 3; i++)
    {
      msize[i] = ut_num_d2ri (In.msize * dsize[i] / avdsize);
      if (msize[i] == 0)
	msize[i] = 1;
    }
  }
  else
    ut_array_1d_int_memcpy (msize, 3, In.msize3);

  ut_free_1d (dsize);

  return;
}
