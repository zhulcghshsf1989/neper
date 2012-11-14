/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#define frand() ((double) rand() / (RAND_MAX+1.0))

#include"RegDistrib.h"

void
CubeDistrib (struct GERMSET *pGermSet, double *size)
{
  int x, y, z, j, nb;
  double* coo = ut_alloc_1d (4);

  /* IniGermsCoo initializes the variable: GermsCoo */
  (*pGermSet).GermsCoo = ut_alloc_2d ((*pGermSet).N1d * (*pGermSet).N1d
				      * (*pGermSet).N1d + 1, 4);

  /* GermsCoo is filled line by line. */
  nb = 0;
  for (x = 1; x <= (*pGermSet).N1d; x++)
  {
    coo[1] = size[1] + (x - 0.5) / (*pGermSet).N1d * (size[2] - size[1]);
    for (y = 1; y <= (*pGermSet).N1d; y++)
    {
      coo[2] = size[3] + (y - 0.5) / (*pGermSet).N1d * (size[4] - size[3]);
      for (z = 1; z <= (*pGermSet).N1d; z++)
      {
	coo[3] = size[5] + (z - 0.5) / (*pGermSet).N1d * (size[6] - size[5]);
	nb++;
	for (j = 1; j <= 3; j++)
	  (*pGermSet).GermsCoo[nb][j] = coo[j];
      }
    }
  }

  ut_free_1d (coo);

  return;
}

void
TruOctaDistrib (struct GERMSET *pGermSet, double *size)
{
  int x, y, z, i, j, nb;
  /* double eps; */
  int n = ut_num_d2ri ((*pGermSet).N1d);
  double* coo = ut_alloc_1d (4);
  double* r = ut_alloc_1d (4);

  /* IniGermsCoo initializes the variable: GermsCoo */
  (*pGermSet).GermsCoo = ut_alloc_1d_pdouble (1);

  /* GermsCoo is filled line by line. */
  nb = 0;
  for (x = 1; x < n; x++)
  {
    coo[1] = size[1] + (x - 0.5) / (n - 1) * (size[2] - size[1]);
    for (y = 1; y < n; y++)
    {
      coo[2] = size[3] + (y - 0.5) / (n - 1) * (size[4] - size[3]);
      for (z = 1; z < n; z++)
      {
	coo[3] = size[5] + (z - 0.5) / (n - 1) * (size[6] - size[5]);
	nb++;
	(*pGermSet).GermsCoo = ut_realloc_2d_addline ((*pGermSet).GermsCoo, nb + 1, 4);
	for (j = 1; j <= 3; j++)
	  (*pGermSet).GermsCoo[nb][j] = coo[j];
      }
    }
  }

  for (x = 0; x < n; x++)
  {
    coo[1] = size[1] + (double)x / (n - 1) * (size[2] - size[1]);
    for (y = 0; y < n; y++)
    {
      coo[2] = size[3] + (double)y / (n - 1) * (size[4] - size[3]);
      for (z = 0; z < n; z++)
      {
	coo[3] = size[5] + (double)z / (n - 1) * (size[6] - size[5]);
	nb++;
	(*pGermSet).GermsCoo = ut_realloc_2d_addline ((*pGermSet).GermsCoo, nb + 1, 4);
	for (j = 1; j <= 3; j++)
	  (*pGermSet).GermsCoo[nb][j] = coo[j];
      }
    }
  }

  (*pGermSet).N = nb;

  for (i = 1; i <= (*pGermSet).N; i++)
    for (j = 1; j <= 3; j++)
    {
      if ((*pGermSet).GermsCoo[i][j] < size[2 * j - 1] + 1e-8)
	(*pGermSet).GermsCoo[i][j] = size[2 * j - 1] + 1e-8;
      else if ((*pGermSet).GermsCoo[i][j] > size[2 * j] - 1e-8)
	(*pGermSet).GermsCoo[i][j] = size[2 * j] - 1e-8;
    }

  /*
  for (i = 1; i <= (*pGermSet).N; i++)
  {
    for (j = 1; j <= 3; j++)
      printf ("%f ", (*pGermSet).GermsCoo[i][j]);
    printf ("\n");
  }

  for (i = 1; i <= (*pGermSet).N; i++)
  {
    ol_nb_max_rtheta_mis (frand (), frand (), frand (), 1e-6, r + 1, &eps);
    for (j = 1; j <= 3; j++)
    (*pGermSet).GermsCoo[i][j] += eps * r[j] * (size[2 * j] - size[2 * j - 1]);
  }
  */

  ut_free_1d (coo);
  ut_free_1d (r);

  return;
}
