/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ_rand.h"
#include<gsl/gsl_rng.h>

/* IniGermCoo initializes the variable: GermCoo */
void
IniGermCoo (char* morpho, int N, int Random, const double* size,
             struct POLY Domain, double **GermCoo)
{
  int i, j;
  gsl_rng *r;

  /* random number generator initialization */
  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, Random);

  /* GermCoo is filled line by line. */
  if (strcmp (morpho, "equiaxed") == 0
   || strcmp (morpho, "poisson")  == 0)
  {
    for (i = 1; i <= N; i++)
    {
      for (j = 0; j < 3; j++)
	GermCoo[i][j] =
	  size[2 * j + 1] + gsl_rng_uniform (r) * (size[2 * j + 2] -
						   size[2 * j + 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnarx") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][0] = (size[2] + size[1]) / 2;
      for (j = 1; j < 3; j++)
	GermCoo[i][j] =
	  size[2 * j + 1] + gsl_rng_uniform (r) * (size[2 * j + 2] -
						   size[2 * j + 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnary") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][1] = (size[4] + size[3]) / 2;
      for (j = 0; j < 3; j+=2)
	GermCoo[i][j] =
	  size[2 * j + 1] + gsl_rng_uniform (r) * (size[2 * j + 2] -
						   size[2 * j + 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnarz") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][2] = (size[6] + size[5]) / 2;
      for (j = 0; j < 2; j++)
	GermCoo[i][j] =
	  size[2 * j + 1] + gsl_rng_uniform (r) * (size[2 * j + 2] -
						   size[2 * j + 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bamboox") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][0] =
	size[1] + gsl_rng_uniform (r) * (size[2] - size[1]);
      GermCoo[i][1] = (size[4] + size[3]) / 2;
      GermCoo[i][2] = (size[6] + size[5]) / 2;
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bambooy") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][0] = (size[2] + size[1]) / 2;
      GermCoo[i][1] =
	size[3] + gsl_rng_uniform (r) * (size[4] - size[3]);
      GermCoo[i][2] = (size[6] + size[5]) / 2;
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bambooz") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermCoo[i][0] = (size[2] + size[1]) / 2;
      GermCoo[i][1] = (size[4] + size[3]) / 2;
      GermCoo[i][2] =
	size[5] + gsl_rng_uniform (r) * (size[6] - size[5]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermCoo[i]) == 0)
	i--;
    }
  }
  else
  {
    ut_print_message (2, 2, "Morphology unknown (%s)\n", morpho);
    abort ();
  }

  gsl_rng_free (r);

  return;
}
