/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RandDistrib.h"

/* IniGermsCoo initializes the variable: GermsCoo */
void
IniGermsCoo (char* morpho, int N, int Random, const double* size,
             struct POLY Domain, double **GermsCoo)
{
  int i, j;
  gsl_rng *r;

  /* random number generator initialization */
  r = gsl_rng_alloc (gsl_rng_ranlxd2);
  gsl_rng_set (r, Random);

  /* GermsCoo is filled line by line. */
  if (strcmp (morpho, "equiaxed") == 0
   || strcmp (morpho, "poisson")  == 0)
  {
    for (i = 1; i <= N; i++)
    {
      for (j = 1; j <= 3; j++)
	GermsCoo[i][j] =
	  size[2 * j - 1] + gsl_rng_uniform (r) * (size[2 * j] -
						   size[2 * j - 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnarx") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][1] = (size[2] + size[1]) / 2;
      for (j = 2; j <= 3; j++)
	GermsCoo[i][j] =
	  size[2 * j - 1] + gsl_rng_uniform (r) * (size[2 * j] -
						   size[2 * j - 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnary") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][2] = (size[4] + size[3]) / 2;
      for (j = 1; j <= 3; j+=2)
	GermsCoo[i][j] =
	  size[2 * j - 1] + gsl_rng_uniform (r) * (size[2 * j] -
						   size[2 * j - 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "columnarz") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][3] = (size[6] + size[5]) / 2;
      for (j = 1; j <= 2; j++)
	GermsCoo[i][j] =
	  size[2 * j - 1] + gsl_rng_uniform (r) * (size[2 * j] -
						   size[2 * j - 1]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bamboox") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][1] =
	size[1] + gsl_rng_uniform (r) * (size[2] - size[1]);
      GermsCoo[i][2] = (size[4] + size[3]) / 2;
      GermsCoo[i][3] = (size[6] + size[5]) / 2;
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bambooy") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][1] = (size[2] + size[1]) / 2;
      GermsCoo[i][2] =
	size[3] + gsl_rng_uniform (r) * (size[4] - size[3]);
      GermsCoo[i][3] = (size[6] + size[5]) / 2;
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
	i--;
    }
  }
  else if (strcmp (morpho, "bambooz") == 0)
  {
    for (i = 1; i <= N; i++)
    {
      GermsCoo[i][1] = (size[2] + size[1]) / 2;
      GermsCoo[i][2] = (size[4] + size[3]) / 2;
      GermsCoo[i][3] =
	size[5] + gsl_rng_uniform (r) * (size[6] - size[5]);
      if (Domain.FaceQty > 0 && neut_poly_point_in (Domain, GermsCoo[i] + 1) == 0)
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
