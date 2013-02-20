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
      int varqty = 1;
      char** vars = ut_alloc_2d_char (1, 10);
      double* vals = ut_alloc_1d (1);
      double res;

      strcpy (vars[0], "vol");
      vals[0] =  M_PI * rad * rad * h;

      ut_math_eval (In.n[1], 1, vars, vals, &res);
      res = ut_num_max (res, 1);

      ut_free_2d_char (vars, varqty);
      ut_free_1d (vals);

      rcl2cl (1, M_PI * rad * rad * h, ut_num_d2ri (res), NULL, &cl);

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
  (*pDomain).Level = 0;

  (*pDomain).VerDom = ut_alloc_2d_int ((*pDomain).VerQty + 1, 2);
  (*pDomain).EdgeDom = ut_alloc_2d_int ((*pDomain).EdgeQty + 1, 2);
  (*pDomain).FaceDom = ut_alloc_2d_int ((*pDomain).FaceQty + 1, 2);
  ut_array_2d_int_set ((*pDomain).VerDom + 1, (*pDomain).VerQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).EdgeDom + 1, (*pDomain).EdgeQty, 2, -1);
  ut_array_2d_int_set ((*pDomain).FaceDom + 1, (*pDomain).FaceQty, 2, -1);

  ut_free_1d (size);
  neut_poly_free (&Poly);

  return;
}
