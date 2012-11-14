/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"GermDistrib.h"

void
GermDistrib (struct IN In, struct POLY Domain, struct GERMSET *pGermSet)
{
  double* size = ut_alloc_1d (7);
  char** parts = NULL;
  int partqty;

  size[2] = In.domainparms[0];
  size[4] = In.domainparms[1];
  size[6] = In.domainparms[2];

  ut_string_separate ((*pGermSet).morpho, '|', &parts, &partqty);

  /* Distributing the germs within the domain */
  if (parts[0][0] == '@')
    DistribFromFile (parts[0] + 1, (*pGermSet).N, &((*pGermSet).GermsCoo));
  else if (strcmp ((*pGermSet).morpho, "tocta") == 0)
  {
    if (! strcmp (In.domain, "cube"))
      TruOctaDistrib (pGermSet, size);
    else
    {
      ut_print_message (2, 0, "This morphology (%s) cannot be used with a non-cube domain (%s)\n", (*pGermSet).morpho, In.domain);
      abort ();
    }
  }
  else // (strcmp ((*pGermSet).morpho, "poisson") == 0)
    RandDistrib (pGermSet, Domain);

  if ((*pGermSet).randomize > 0)
    net_germ_randomize (pGermSet, size, parts[0]);

  ut_free_1d (size);
  ut_free_2d_char (parts, partqty);

  return;
}
