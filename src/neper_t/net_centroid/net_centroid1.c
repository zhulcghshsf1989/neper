/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_centroid.h"

int
net_centroid (struct IN In, struct TESS Tess, struct GERMSET* pGermSet, double* prdistmax)
{
  int status, i;
  double* c = ut_alloc_1d (3);
  double dist, distmax, geovol, avrad;
  struct GEO Geo;

  neut_geo_set_zero (&Geo);

  neut_tess_geo (Tess, &Geo);

  neut_geo_volume (Geo, &geovol);
  avrad = pow ((geovol / Geo.PolyQty) / (4/3 * M_PI), 0.33333333);

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
  for (i = 1; i <= Geo.PolyQty; i++)
  {
    neut_geo_poly_centroid (Geo, i, c);
    dist = ut_space_dist (c, (*pGermSet).GermsCoo[i] + 1);
    distmax = ut_num_max (dist, distmax);

    int j;
    for (j = 0; j < cooqty; j++)
    {
      id = coo[j];
      (*pGermSet).GermsCoo[i][id + 1] = (*pGermSet).GermsCoo[i][id + 1] * (1 - In.centroidfact)
				 + c[id] * In.centroidfact;
    }
  }

  neut_geo_free (&Geo);

  ut_free_1d (c);

  (*prdistmax) = distmax / avrad;

  if ((*prdistmax) < In.centroidconv)
    status = 0;
  else
    status = 1;

  ut_free_1d_int (coo);

  return status;
}
