/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_centroid.h"

int
net_centroid (double centroidfact, double centroidconv, struct TESL Tesl, struct GERMSET* pGermSet, double* prdistmax)
{
  int status, i;
  double* c = ut_alloc_1d (3);
  double dist, distmax, tessvol, avrad;
  struct TESS net_poly_tesl;

  neut_tess_set_zero (&net_poly_tesl);

  neut_tesl_tess (Tesl, &net_poly_tesl);

  neut_tess_volume (net_poly_tesl, &tessvol);
  avrad = pow ((tessvol / net_poly_tesl.PolyQty) / (4/3 * M_PI), 0.33333333);

  int id, cooqty = 0;
  int* coo = ut_alloc_1d_int (3);
  if (! strcmp ((*pGermSet).morpho, "columnarx"))
  {
    cooqty = 2;
    coo[0] = 1;
    coo[1] = 2;
  }
  else if (! strcmp ((*pGermSet).morpho, "columnary"))
  {
    cooqty = 2;
    coo[0] = 0;
    coo[1] = 2;
  }
  else if (! strcmp ((*pGermSet).morpho, "columnarz"))
  {
    cooqty = 2;
    coo[0] = 0;
    coo[1] = 1;
  }
  else if (! strcmp ((*pGermSet).morpho, "bamboox"))
  {
    cooqty = 1;
    coo[0] = 0;
  }
  else if (! strcmp ((*pGermSet).morpho, "bambooy"))
  {
    cooqty = 1;
    coo[0] = 1;
  }
  else if (! strcmp ((*pGermSet).morpho, "bambooz"))
  {
    cooqty = 1;
    coo[0] = 2;
  }
  else
  {
    cooqty = 3;
    coo[0] = 0;
    coo[1] = 1;
    coo[2] = 2;
  }

  distmax = 0;
  for (i = 1; i <= net_poly_tesl.PolyQty; i++)
  {
    neut_tess_poly_centroid (net_poly_tesl, i, c);
    dist = ut_space_dist (c, (*pGermSet).GermsCoo[i] + 1);
    distmax = ut_num_max (dist, distmax);

    int j;
    for (j = 0; j < cooqty; j++)
    {
      id = coo[j];
      (*pGermSet).GermsCoo[i][id + 1] = (*pGermSet).GermsCoo[i][id + 1] * (1 - centroidfact)
				 + c[id] * centroidfact;
    }
  }

  neut_tess_free (&net_poly_tesl);

  ut_free_1d (c);

  (*prdistmax) = distmax / avrad;

  if ((*prdistmax) < centroidconv)
    status = 0;
  else
    status = 1;

  ut_free_1d_int (coo);

  return status;
}
