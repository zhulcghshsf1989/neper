/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "NSite.h"

void
DistribPoints (double *MaxSize, double *MinSize, int Qty, double **Data,
	       gsl_rng * r)
{
  int i, j, id;
  double *coo = ut_alloc_1d (4);

  /* ckecking the zone sizes */

  /* printf("DISTRIB from %lf to %lf: %d germs\n",-MinSize[1],-MaxSize[1],Qty); */

  for (i = 1; i <= 3; i++)
  {
    if (MinSize[2 * i - 1] < MaxSize[2 * i - 1])
      abort ();
    if (MinSize[2 * i] > MaxSize[2 * i])
      abort ();
  }

  id = 0;
  while (id != Qty)
  {
    for (i = 1; i <= 3; i++)
      coo[i] =
	MaxSize[2 * i - 1] + gsl_rng_uniform (r) * (MaxSize[2 * i] -
						    MaxSize[2 * i - 1]);

    if (coo[1] < MinSize[1] || coo[1] > MinSize[2]
	|| coo[2] < MinSize[3] || coo[2] > MinSize[4]
	|| coo[3] < MinSize[5] || coo[3] > MinSize[6])
    {
      id++;
      for (j = 1; j <= 3; j++)
	Data[id][j] = coo[j];
    }
  }

  ut_free_1d (coo);

  return;
}

