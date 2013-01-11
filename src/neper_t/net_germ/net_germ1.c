/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ.h"

void
net_germ (struct IN In, struct TESS Domain, struct GERMSET* pGermSet)
{
  struct POLY Poly;

  neut_poly_set_zero (&Poly);

  net_tess_poly (Domain, 1, &Poly);

  (*pGermSet).N = In.n;
  (*pGermSet).N1d = In.n;
  (*pGermSet).Id = In.id;
  (*pGermSet).NDensity = (*pGermSet).N;
  (*pGermSet).Size = ut_alloc_1d (7);
  neut_poly_bbox (Poly, (*pGermSet).Size);

  (*pGermSet).morpho = ut_alloc_1d_char (strlen (In.morpho) + 1);
  strcpy ((*pGermSet).morpho, In.morpho);

  /* Distributing the germs within the domain */
  if (In.morpho[0] == '@')
    DistribFromFile (In.morpho + 1, In.n, &((*pGermSet).GermsCoo));
  else if (! strcmp (In.morpho, "tocta")
        || ! strcmp (In.morpho, "cube")
        || ! strcmp (In.morpho, "dodeca"))
  {
    if (strcmp (In.domain, "cube") != 0)
    {
      ut_print_message (2, 0, "This morphology (%s) cannot be used with a non-cube domain (%s)\n", In.morpho, In.domain);
      abort ();
    }

    if (! strcmp (In.morpho, "tocta"))
      TruOctaDistrib (Poly, pGermSet);
    else if (! strcmp (In.morpho, "cube"))
      CubeDistrib (Poly, pGermSet);
    else
      abort ();
  }
  else // morpho = poisson, columnarx, ...
    RandDistrib (In, pGermSet, Poly);

  DefGermSet (In, pGermSet);

  // Randomize, if needed ----------------------------------------------
  
  if (In.randomize > 0)
    net_germ_randomize (In, pGermSet, Poly, In.morpho);

  neut_poly_free (&Poly);

  return;
}
