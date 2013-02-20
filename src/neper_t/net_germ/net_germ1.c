/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_germ.h"

void
net_germ (struct IN In, struct TESS Tess, int poly, struct GERMSET* pGSet)
{
  // Initializing variables --------------------------------------------

  struct POLY Poly;

  neut_poly_set_zero (&Poly);
  net_tess_poly (Tess, poly, &Poly);

  neut_germset_free (pGSet);

  // Testing out input data --------------------------------------------

  // Regular distributions are only available for a cubic domain
  if ((   ! strcmp (In.morpho[Tess.Level + 1], "tocta")
       || ! strcmp (In.morpho[Tess.Level + 1], "cube")
       || ! strcmp (In.morpho[Tess.Level + 1], "dodeca"))
      &&    strcmp (In.domain, "cube") != 0)
      ut_print_message
	(2, 0, "This morphology (%s) cannot be used with a non-cube domain (%s)\n",
	 In.morpho[Tess.Level + 1], In.domain);

  // Setting up properties of the germset (number of germs, etc.) ------

  net_germ_prop (In, Tess, poly, pGSet);

  // Distributing germs ------------------------------------------------

  if (! strcmp ((*pGSet).morpho, "tocta") || ! strcmp ((*pGSet).morpho, "cube"))
    net_germ_reg (Poly, pGSet);
  else if ((*pGSet).morpho[0] == '@')
    net_germ_custom ((*pGSet).morpho + 1, Poly, pGSet);
  else
  {
    net_germ_seed (Tess.Level, poly, NULL, pGSet);
    net_germ_rand (In, Poly, pGSet);
  }

  // Distributing germs outside the domain, if needed
  if (! strcmp (In.ttype, "periodic")
   || ! strcmp (In.ttype, "subdomain"))
    net_germ_neigh (In, pGSet);

  // Randomize germ distribution ---------------------------------------
  
  if (In.randomize > 0)
    net_germ_randomize (In, pGSet, Poly);

  // Free'ing memory ---------------------------------------------------

  neut_poly_free (&Poly);

  return;
}
