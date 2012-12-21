/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_domain.h"

void
net_domain (struct IN In, struct TESS* pDomain)
{
  double* size = ut_alloc_1d (7);
  struct POLY Poly;

  neut_poly_set_zero (&Poly);

  if (! strcmp (In.domain, "cube"))
  {
    size[2] = In.domainparms[0];
    size[4] = In.domainparms[1];
    size[6] = In.domainparms[2];

    net_domain_cube (size, &Poly);
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
      rcl2cl (1, M_PI * rad * rad * h, In.n, NULL, &cl);
      qty = (int) floor (2 * M_PI * rad / cl);
      qty = ut_num_max_int (qty, 5);
      qty += 2;
    }

    double** eq = ut_alloc_2d (qty, 4);

    net_domain_cylinder_planes (h, rad, qty - 2, eq);
    net_domain_clip (&Poly, eq, qty);
    ut_free_2d (eq, qty);
  }
  else if (! strcmp (In.domain, "planes"))
  {
    int i, qty; 
    FILE* file = ut_file_open (In.domainparms2, "r");

    if (fscanf (file, "%d", &qty) != 1)
      abort ();

    double** eq = ut_alloc_2d (qty, 4);

    for (i = 0; i < qty; i++)
    {
      ut_array_1d_fscanf (file, eq[i] + 1, 3);
      if (fscanf (file, "%lf", &(eq[i][0])) != 1)
	abort ();
      ut_array_1d_scale (eq[i], 4, 1. / ut_vector_norm (eq[i] + 1));
    }

    net_domain_clip (&Poly, eq, qty);

    ut_free_2d (eq, qty);
    
    ut_file_close (file, In.domainparms2, "r");
  }
  else if (! strcmp (In.domain, "tesspoly"))
  {
    int id = ut_num_d2ri (In.domainparms[0]);
    struct TESS Tess;
    neut_tess_set_zero (&Tess);

    FILE* file = ut_file_open (In.domainparms2, "r");
    neut_tess_fscanf (file, &Tess);
    net_tess_poly (Tess, id, &Poly);
    ut_file_close (file, In.domainparms2, "r");
  }
  else
  {
    ut_print_message (2, 0, "Domain type unknown (%s)\n", In.domain);
    abort ();
  }

  net_poly_tess (Poly, pDomain);

  ut_free_1d (size);
  neut_poly_free (&Poly);

  return;
}
