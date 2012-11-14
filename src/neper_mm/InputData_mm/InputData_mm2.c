/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_mm.h"

void
TreatArg_mm (int fargc, char **fargv, int argc, char **argv,
	     struct IN *pIn, struct GERMSET* pGermSet)
{
  SetDefaultOptions_mm (pIn, pGermSet);

  /* Reading options. */
  SetOptions_mm (pIn, pGermSet, fargc, fargv);
  SetOptions_mm (pIn, pGermSet, argc, argv);

  (*pGermSet).Size = ut_alloc_1d (7);
  (*pGermSet).Size[2] = 1;
  (*pGermSet).Size[4] = 1;
  (*pGermSet).Size[6] = 1;

  if ((*pIn).input == 0)
  {
    (*pGermSet).morpho = ut_alloc_1d_char (100);
    sprintf ((*pGermSet).morpho, "poisson");
    (*pGermSet).N  = (*pGermSet).NDensity;
    (*pGermSet).Id = (*pGermSet).Id;
  }

  /* Checking the input options */
  if ((*pIn).input == -1)
    ut_arg_badarg ();
  else if ((*pIn).input == 1)
  {				
    if ((*pGermSet).Id != -1)
    {
      printf ("id    = %d (should be != -1)\n",  (*pGermSet).Id);
      ut_arg_badarg ();
    }

    if (ut_file_exist ((*pIn).ingeo) == 0)	/* and the file must exist. */
      ut_file_open ((*pIn).ingeo, "r");
  }
  else if ((*pIn).input == 2)
  {
    net_n1d_morpho_n ((*pGermSet).N1d, (*pGermSet).morpho, &((*pGermSet).N));
    (*pGermSet).NDensity = (*pGermSet).N;
  }

  if (! strcmp ((*pGermSet).ttype, "subdomain") == 0
   && ((*pIn).nsitesize > 0 || (*pIn).nsite != 0))
  {
    ut_print_messagewnc (2, 72,
		"  `-nsiteXXX' options can be used only with `-ttype subdomain'.");
    abort ();
  }

  /* Writing file names */
  if ((*pIn).body == NULL)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if ((*pIn).ingeo != NULL)
      ut_string_body ((*pIn).ingeo, (*pIn).body);
    else if ((*pIn).loadmesh != NULL)
      ut_string_body ((*pIn).loadmesh, (*pIn).body);
    else if ((*pIn).input == 2)
      sprintf ((*pIn).body, "%s%d", (*pGermSet).morpho, (*pGermSet).N1d);
    else if ((*pGermSet).Id != -1)
      sprintf ((*pIn).body, "n%d-id%d", (*pGermSet).NDensity, (*pGermSet).Id);
    else
      sprintf ((*pIn).body, "n%d-rand", (*pGermSet).NDensity);
  }

  (*pIn).geo = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).gmshgeo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).msh = ut_string_addextension ((*pIn).body, ".msh");
  (*pIn).geof = ut_string_addextension ((*pIn).body, ".geof");
  (*pIn).abq = ut_string_addextension ((*pIn).body, ".inp");
  (*pIn).oin = ut_string_addextension ((*pIn).body, ".oin");
  (*pIn).nsitefile = ut_string_addextension ((*pIn).body, ".nsite");

  return;
}
