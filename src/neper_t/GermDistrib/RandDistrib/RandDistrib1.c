/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"RandDistrib.h"

/* IniVariables initializes the program variables related to the 
 * previously loaded input data and internal parameters. They are
 * described in the mp.h file. */
void
RandDistrib (struct GERMSET *pGermSet, struct POLY Domain)
{
  char** parts = NULL;
  int partqty;

  ut_string_separate ((*pGermSet).morpho, '|', &parts, &partqty);

  /* Initializing the variable: Random */
  (*pGermSet).Random = neut_rand_nnid2rand ((*pGermSet).N, (*pGermSet).Id);

  if (! strcmp ((*pGermSet).ttype, "subdomain"))
  {
    (*pGermSet).NDensity = (*pGermSet).N;
    (*pGermSet).N =
      neut_rand_poisson ((double) (*pGermSet).NDensity, (int) (*pGermSet).Random, 1);
  }

  double* size = ut_alloc_1d (7);

  neut_poly_boundingbox (Domain, size);

  /* IniGermsCoo initializes the variable: GermsCoo */
  (*pGermSet).GermsCoo = ut_alloc_2d ((*pGermSet).N + 1, 4);

  IniGermsCoo (parts[0], (*pGermSet).N, (*pGermSet).Random,
	       size, Domain, (*pGermSet).GermsCoo);

  ut_free_1d (size);
  ut_free_2d_char (parts, partqty);

  return;
}

void
DistribFromFile (char* filename, int N, double*** pGermsCoo)
{
  FILE *file = ut_file_open (filename, "r");
  int i;

  (*pGermsCoo) = ut_alloc_2d (N + 1, 4);

  for (i = 1; i <= N; i++)
    ut_array_1d_fscanf (file, (*pGermsCoo)[i] + 1, 3);

  ut_file_close (file, filename, "r");

  return;
}
