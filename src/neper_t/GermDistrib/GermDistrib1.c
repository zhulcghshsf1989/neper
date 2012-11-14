/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"GermDistrib.h"

void
GermDistrib (struct IN In, struct POLY Domain, struct GERMSET *pGermSet)
{
  int i, j;
  double eps; 
  double* r = ol_r_alloc ();
  double* size2 = ut_alloc_1d (7);
  char** parts = NULL;
  int partqty;

  size2[2] = In.domainparms[0];
  size2[4] = In.domainparms[1];
  size2[6] = In.domainparms[2];

  ut_string_separate ((*pGermSet).morpho, '|', &parts, &partqty);

  /* Distributing the germs within the domain */
  if (parts[0][0] == '@')
    DistribFromFile (parts[0] + 1, (*pGermSet).N, &((*pGermSet).GermsCoo));
  else if (strcmp ((*pGermSet).morpho, "tocta") == 0)
  {
    if (! strcmp (In.domain, "cube"))
      TruOctaDistrib (pGermSet, size2);
    else
    {
      ut_print_message (2, 0, "This morphology (%s) cannot be used with a non-cube domain (%s)\n", (*pGermSet).morpho, In.domain);
      abort ();
    }
  }
  else // (strcmp ((*pGermSet).morpho, "poisson") == 0)
    RandDistrib (pGermSet, Domain);

  if ((*pGermSet).randomize > 0)
  {
    int* dir = ut_alloc_1d_int (4);

    if (strcmp ((*pGermSet).randomizedir, "default") == 0)
    {
      if (strncmp (parts[0], "columnar", 8) == 0)
      {
	ut_array_1d_int_set (dir + 1, 3, 1);

	if (strcmp (parts[0] + 8, "x") == 0)
	  dir[1] = 0;
	else if (strcmp (parts[0] + 8, "y") == 0)
	  dir[2] = 0;
	else if (strcmp (parts[0] + 8, "z") == 0)
	  dir[3] = 0;
	else
	  abort ();
      }
      else if (strncmp (parts[0], "bamboo", 6) == 0)
      {
	ut_array_1d_int_set (dir + 1, 3, 0);

	if (strcmp (parts[0] + 6, "x") == 0)
	  dir[1] = 1;
	else if (strcmp (parts[0] + 6, "y") == 0)
	  dir[2] = 1;
	else if (strcmp (parts[0] + 6, "z") == 0)
	  dir[3] = 1;
	else
	  abort ();
      }
      else
	ut_array_1d_int_set (dir + 1, 3, 1);
    }
    else
    {
      ut_array_1d_int_set (dir + 1, 3, 0);

      if (ut_string_inlist ((*pGermSet).randomizedir, ',', "x") == 1)
	dir[1] = 1;
      if (ut_string_inlist ((*pGermSet).randomizedir, ',', "y") == 1)
	dir[2] = 1;
      if (ut_string_inlist ((*pGermSet).randomizedir, ',', "z") == 1)
	dir[3] = 1;
    }

    gsl_rng *rr = gsl_rng_alloc (gsl_rng_ranlxd2);
    gsl_rng_set (rr, (*pGermSet).randomize2);

    dir[0] = ut_array_1d_int_sum (dir + 1, 3);

    double val;

    for (i = 1; i <= (*pGermSet).N; i++)
    {
      // FIX R HERE FOR THE NON THREE D CASES
      if (dir[0] == 3)
	ol_nb_r (gsl_rng_uniform (rr), gsl_rng_uniform (rr), r);
      else if (dir[0] == 2)
      {
	ol_nb_r (0.5, gsl_rng_uniform (rr), r);

	if (dir[1] == 0)
	  ut_array_1d_rotate (r, 3, 2);
	else if (dir[2] == 0)
	  ut_array_1d_rotate (r, 3, 1);

	gsl_rng_uniform (rr); // this is to trash one
      }
      else if (dir[0] == 1)
      {
	ol_r_set_zero (r);
	for (j = 1; j <= 3; j++)
	{
	  val = gsl_rng_uniform (rr);

	  if (dir[j] == 1)
	    r[j - 1] = (val < 0.5) ? -1 : 1;
	}
      }
      else
	ol_r_set_zero (r);

      eps = (*pGermSet).randomize * gsl_rng_uniform (rr);

      for (j = 1; j <= 3; j++)
	if (dir[j] == 1)
	{
	  (*pGermSet).GermsCoo[i][j] += eps * r[j - 1] * (size2[2 * j] - size2[2 * j - 1]);
	  if ((*pGermSet).GermsCoo[i][j] < size2[2 * j - 1])
	    (*pGermSet).GermsCoo[i][j] = size2[2 * j - 1] + 1e-6;
	  if ((*pGermSet).GermsCoo[i][j] > size2[2 * j])
	    (*pGermSet).GermsCoo[i][j] = size2[2 * j] - 1e-6;
	}
    }
    
    gsl_rng_free (rr);
    ut_free_1d_int (dir);
  }

  ol_r_free (r);

  ut_free_1d (size2);

  ut_free_2d_char (parts, partqty);

  return;
}
