/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ.h"

void
net_germ_seed (int level, int poly, char* width, struct GERMSET* pGSet)
{
  double w;
  int N;

  if (width == NULL)
    N = (*pGSet).N;
  else
  {
    if (sscanf (width, "%lf", &w) != 1)
      ut_error_reportbug ();

    N = ut_num_d2ri (1 / w);
  }

  (*pGSet).Random = neut_rand_nnid2rand (N, (*pGSet).Id, level, poly);

  if ((*pGSet).Random < 0)
    ut_error_reportbug ();

  return;
}

void
net_germ_prop (struct IN In, struct TESS Tess, int poly, struct GERMSET* pGSet)
{
  int varqty = 1;
  char** vars = NULL;
  double* vals = NULL;
  double res;

  (*pGSet).morpho = ut_alloc_1d_char (strlen (In.morpho[Tess.Level + 1]) + 1);
  strcpy ((*pGSet).morpho, In.morpho[Tess.Level + 1]);

  if (strcmp ((*pGSet).morpho, "lamellae") != 0)
  {
    varqty = 1;
    vars = ut_alloc_2d_char (1, 10);
    vals = ut_alloc_1d (1);

    strcpy (vars[0], "vol");
    neut_tess_poly_volume (Tess, poly, &(vals[0]));

    ut_math_eval (In.n[Tess.Level + 1], 1, vars, vals, &res);
    (*pGSet).N = ut_num_d2ri (ut_num_max (1, res));

    ut_free_2d_char (vars, varqty);
    ut_free_1d (vals);
  }
  else
  {
    (*pGSet).N = -1;
  }

  (*pGSet).N1d = (*pGSet).N;
  (*pGSet).NDensity = (*pGSet).N;

  (*pGSet).Id = In.id[Tess.Level + 1];

  return;
}

void
net_germ_randomize (struct IN In, struct GERMSET* pGSet, struct POLY Domain)
{
  int i, j, dirsum;
  double eps;
  double* r = ol_r_alloc ();
  double* bbox = ut_alloc_1d (7);

  neut_poly_bbox (Domain, bbox);

  if (In.randomize > 0)
  {
    int* dir = ut_alloc_1d_int (3);

    if (strcmp (In.randomizedir, "default") == 0)
    {
      if (strncmp ((*pGSet).morpho, "columnar", 8) == 0)
      {
	ut_array_1d_int_set (dir, 3, 1);

	if ((*pGSet).morpho[8] == 'x')
	  dir[0] = 0;
	else if ((*pGSet).morpho[8] == 'y')
	  dir[1] = 0;
	else if ((*pGSet).morpho[8] == 'z')
	  dir[2] = 0;
	else
	  abort ();
      }
      else if (strncmp ((*pGSet).morpho, "bamboo", 6) == 0)
      {
	ut_array_1d_int_set (dir, 3, 0);

	if ((*pGSet).morpho[6] == 'x')
	  dir[0] = 1;
	else if ((*pGSet).morpho[6] == 'y')
	  dir[1] = 1;
	else if ((*pGSet).morpho[6] == 'z')
	  dir[2] = 1;
	else
	  abort ();
      }
      else
	ut_array_1d_int_set (dir, 3, 1);
    }
    else
    {
      ut_array_1d_int_set (dir, 3, 0);

      if (ut_string_inlist (In.randomizedir, ',', "x") == 1)
	dir[0] = 1;
      if (ut_string_inlist (In.randomizedir, ',', "y") == 1)
	dir[1] = 1;
      if (ut_string_inlist (In.randomizedir, ',', "z") == 1)
	dir[2] = 1;
    }

    gsl_rng *rr = gsl_rng_alloc (gsl_rng_ranlxd2);
    gsl_rng_set (rr, In.randomize2);

    dirsum = ut_array_1d_int_sum (dir, 3);

    double val;

    for (i = 1; i <= (*pGSet).N; i++)
    {
      // FIX R HERE FOR THE NON THREE D CASES
      if (dirsum == 3)
	ol_nb_r (gsl_rng_uniform (rr), gsl_rng_uniform (rr), r);
      else if (dirsum == 2)
      {
	ol_nb_r (0.5, gsl_rng_uniform (rr), r);

	if (dir[0] == 0)
	  ut_array_1d_rotate (r, 3, 2);
	else if (dir[1] == 0)
	  ut_array_1d_rotate (r, 3, 1);

	gsl_rng_uniform (rr); // this is to trash one
      }
      else if (dirsum == 1)
      {
	ol_r_set_zero (r);
	for (j = 0; j < 3; j++)
	{
	  val = gsl_rng_uniform (rr);

	  if (dir[j] == 1)
	    r[j] = (val < 0.5) ? -1 : 1;
	}
      }
      else
	ol_r_set_zero (r);

      eps = In.randomize * gsl_rng_uniform (rr);

      for (j = 0; j < 3; j++)
	if (dir[j] == 1)
	{
	  (*pGSet).GermCoo[i][j] += eps * r[j] * (bbox[2 * j + 2] - bbox[2 * j + 1]);
	  if ((*pGSet).GermCoo[i][j] < bbox[2 * j + 1])
	    (*pGSet).GermCoo[i][j] = bbox[2 * j + 1] + 1e-6;
	  if ((*pGSet).GermCoo[i][j] > bbox[2 * j + 2])
	    (*pGSet).GermCoo[i][j] = bbox[2 * j + 2] - 1e-6;
	}
    }

    gsl_rng_free (rr);
    ut_free_1d_int (dir);
  }

  ol_r_free (r);
  ut_free_1d (bbox);

  return;
}
