/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DomainComp.h"

void
DomainComp (struct IN In, struct GERMSET GermSet, struct POLY *pDomain)
{
  double* size = ut_alloc_1d (7);

  if (! strcmp (In.domain, "cube"))
  {
    size[2] = In.domainparms[0];
    size[4] = In.domainparms[1];
    size[6] = In.domainparms[2];

    CubeDomain (size, pDomain);
  }
  else if (! strcmp (In.domain, "cylinder"))
  {
    double h   = In.domainparms[0];
    double rad = In.domainparms[1] / 2;
    int qty; 

    if (In.domainparms[2] > 0)
      qty = ut_num_d2ri (In.domainparms[2]) + 2;
    else
    {
      double cl;
      rcl2cl (1, M_PI * rad * rad * h, GermSet.N, &cl);
      qty = (int) floor (2 * M_PI * rad / cl);
      qty = ut_num_max_int (qty, 5);
      qty += 2;
    }

    double** eq = ut_alloc_2d (qty, 4);

    net_domain_cylinder_planes (h, rad, qty - 2, eq);
    net_domain_clip (pDomain, eq, qty);
    ut_free_2d (eq, qty);
  }
  else if (! strcmp (In.domain, "planes"))
  {
    int i, qty; 
    FILE* file = ut_file_open (In.domainparms2, "r");

    fscanf (file, "%d", &qty);
    double** eq = ut_alloc_2d (qty, 4);

    for (i = 0; i < qty; i++)
    {
      ut_array_1d_fscanf (file, eq[i] + 1, 3);
      fscanf (file, "%lf", &(eq[i][0]));
      ut_array_1d_scale (eq[i], 4, 1. / ut_vector_norm (eq[i] + 1));
    }

    net_domain_clip (pDomain, eq, qty);

    ut_free_2d (eq, qty);
    
    ut_file_close (file, In.domainparms2, "r");
  }
  else if (! strcmp (In.domain, "tesspoly"))
  {
    int qty;
    double** eq = ut_alloc_2d (1000, 4);
    int id = ut_num_d2ri (In.domainparms[0]);
    struct GEO Geo;
    neut_geo_set_zero (&Geo);

    FILE* file = ut_file_open (In.domainparms2, "r");
    neut_geo_fscanf (file, &Geo);
    net_domain_tesspoly_planes (Geo, id, &qty, eq);
    net_domain_clip (pDomain, eq, qty);

    ut_file_close (file, In.domainparms2, "r");
  }
  else
  {
    ut_print_message (2, 0, "Domain type unknown (%s)\n", In.domain);
    abort ();
  }

  ut_free_1d (size);

  return;
}
