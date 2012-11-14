/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

void
TreatArg_t (int fargc, char **fargv, int argc, char **argv,
            struct IN* pIn, struct GEOPARA* pGeoPara)
{
  SetDefaultOptions_t (pIn, pGeoPara);

  /* Reading options */

  SetOptions_t (pIn, pGeoPara, fargc, fargv);
  SetOptions_t (pIn, pGeoPara, argc, argv);

  /* Testing options */
  if ((*pIn).input == NULL)
  {
    ut_print_message (2, 0, "missing input data.");
    abort ();
  }

  if (! strcmp ((*pIn).input, "n") && (*pIn).id == -1)
    ut_print_messagewnc (1, 72,
		"The identifier (-id) has not been specified; considering a random value ...");

  char** parts = NULL;
  int partqty;

  ut_string_separate ((*pIn).morpho, '|', &parts, &partqty);

  if ((*pIn).id > 0
  && ((strcmp (parts[0], "cube"  ) == 0)
   || (strcmp (parts[0], "dodeca") == 0)
   || (strcmp (parts[0], "tocta" ) == 0)))
  {
    ut_print_message (2, 0,
	      "Options `-id' and `-morpho' are mutually exclusive!");
    abort ();
  }

  /* Writing file names */

  if ((*pIn).body == NULL)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if (! strcmp ((*pIn).input, "n"))
    {
      if ((*pIn).id != -1)
	sprintf ((*pIn).body, "n%d-id%d", (*pIn).n, (*pIn).id);
      else
      {
	if (! strcmp ((*pIn).morpho, "poisson"))
	  sprintf ((*pIn).body, "n%d-rand", (*pIn).n);
	else if ((*pIn).morpho[0] != '@')
	  sprintf ((*pIn).body, "n%d-%s", (*pIn).n, (*pIn).morpho);
	else
	  sprintf ((*pIn).body, "n%d-coo", (*pIn).n);
      }
    }
    else if (! strcmp ((*pIn).input, "tess")
          || ! strcmp ((*pIn).input, "vox"))
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).load));
      ut_string_body ((*pIn).load, (*pIn).body);
    }
    else if (! strcmp ((*pIn).input, "n_reg"))
      sprintf ((*pIn).body, "n%d-%s", (*pIn).n, (*pIn).morpho);
    else
      abort ();
  }

  (*pIn).tess = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).geo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).vox = ut_string_addextension ((*pIn).body, ".vox");
  (*pIn).ply = ut_string_addextension ((*pIn).body, ".ply");
  (*pIn).debug = ut_string_addextension ((*pIn).body, ".debug");

  if ((*pIn).printpointpoly == 1)
    (*pIn).polyid = ut_string_addextension ((*pIn).point, ".polyid");
  else if ((*pIn).printpointpoly == 2)
    (*pIn).polyid = ut_string_paste ((*pIn).point, "-centre.polyid");

  ut_free_2d_char (parts, partqty);

  return;
}
