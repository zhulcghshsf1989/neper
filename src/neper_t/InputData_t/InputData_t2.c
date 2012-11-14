/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

void
TreatArg_t (int fargc, char **fargv, int argc, char **argv,
	    struct IN *pIn, struct GERMSET *pGermSet)
{
  SetDefaultOptions_t (pIn, pGermSet);

  /* Reading options */

  SetOptions_t (pIn, pGermSet, fargc, fargv);
  SetOptions_t (pIn, pGermSet, argc, argv);

  /* Testing options */
  if ((*pIn).input == -1)
  {
    ut_print_message (2, 0, "missing input data.");
    abort ();
  }

  if ((*pIn).input == 0 && (*pGermSet).Id == -1)
    ut_print_messagewnc (1, 72,
		"The identifier (-id) has not been specified; considering a random value ...");

  char** parts = NULL;
  int partqty;

  ut_string_separate ((*pGermSet).morpho, '|', &parts, &partqty);

  // if ((*pIn).input == 2 && strcmp ((*pGermSet).morpho, "tocta") == 0)
  if ((*pIn).input == 2)
    net_n1d_morpho_n ((*pGermSet).N1d, parts[0], &((*pGermSet).N));

  if ((*pGermSet).Id > 0
  && ((strcmp (parts[0], "cube"  ) == 0)
   || (strcmp (parts[0], "dodeca") == 0)
   || (strcmp (parts[0], "tocta" ) == 0)))
  {
    ut_print_message (2, 0,
	      "Options `-id' and `-regular' are mutually exclusive!");
    abort ();
  }

  /* Writing file names */

  if ((*pIn).body == NULL)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if ((*pIn).input == 0)
    {
      if ((*pGermSet).Id != -1)
	sprintf ((*pIn).body, "n%d-id%d", (*pGermSet).N, (*pGermSet).Id);
      else
	sprintf ((*pIn).body, "n%d-rand", (*pGermSet).N);
    }
    else if ((*pIn).input == 1)
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).load));
      ut_string_body ((*pIn).load, (*pIn).body);
    }
    else if ((*pIn).input == 2)
	sprintf ((*pIn).body, "%s%d", parts[0], (*pGermSet).N1d);
    else
      abort ();
  }

  (*pIn).geo = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).gmshgeo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).ply = ut_string_addextension ((*pIn).body, ".ply");
  (*pIn).oin = ut_string_addextension ((*pIn).body, ".oin");
  (*pIn).stt0 = ut_string_addextension ((*pIn).body, ".stt0");
  (*pIn).stt3 = ut_string_addextension ((*pIn).body, ".stt3");
  (*pIn).mast = ut_string_addextension ((*pIn).body, ".mast");
  (*pIn).asy = ut_string_addextension ((*pIn).body, ".asy");
  (*pIn).neigh = ut_string_addextension ((*pIn).body, ".neigh");
  (*pIn).debug = ut_string_addextension ((*pIn).body, ".debug");

  if ((*pIn).printpointpoly == 1)
    (*pIn).polyid = ut_string_addextension ((*pIn).point, ".polyid");
  else if ((*pIn).printpointpoly == 2)
    (*pIn).polyid = ut_string_paste ((*pIn).point, "-centre.polyid");

  ut_free_2d_char (parts, partqty);

  return;
}
