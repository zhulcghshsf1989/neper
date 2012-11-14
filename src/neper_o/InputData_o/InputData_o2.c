/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_o.h"

void
TreatArg_o (int fargc, char **fargv, int argc, char **argv,
	    struct ORI *pOri, struct IN *pIn)
{
  SetDefaultOptions_o (pOri, pIn);

  /* Reading options */
  SetOptions_o (pOri, pIn, fargc, fargv);
  SetOptions_o (pOri, pIn, argc, argv);

  /* Testing options */
  if ((*pIn).input == -1)
    ut_arg_badarg ();

  if (strcmp ((*pIn).descriptor, "e") != 0
   && strcmp ((*pIn).descriptor, "ek") != 0
   && strcmp ((*pIn).descriptor, "er") != 0
   && strcmp ((*pIn).descriptor, "g") != 0
   && strcmp ((*pIn).descriptor, "rtheta") != 0
   && strcmp ((*pIn).descriptor, "R") != 0
   && strcmp ((*pIn).descriptor, "q") != 0)
    ut_print_message (2, 1, "Unknow orientation descriptor `%s'\n",
	(*pIn).descriptor);

  if (strcmp ((*pIn).format, "fev") == 0
   && strcmp ((*pIn).descriptor, "ek") != 0)
    {
      ut_print_message (2, 1, "With format `fev', the descriptor must be `ek'.\n");
      abort ();
    }

  if (strcmp ((*pIn).format, "geof") == 0
   && strcmp ((*pIn).descriptor, "e") != 0)
    {
      ut_print_message (2, 1, "With format `geof', the descriptor must be `e'.\n");
      abort ();
    }

  if ((*pIn).colourspec != NULL)
  {
    if (strcmp ((*pIn).colourspec, "R") != 0)
    {
      ut_print_message (2, 0, "-colour argument must be R (is %s).\n", (*pIn).colourspec);
      abort ();
    }
    if (strcmp ((*pIn).crysym, "cubic") != 0)
    {
      ut_print_message (2, 0, "with -colour R, -crysym must `cubic'.\n");
      abort ();
    }
  }

  /* Writing file names */

  if ((*pIn).body == NULL)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if ((*pIn).input == 0 && ((*pOri).id != -1))
      sprintf ((*pIn).body, "n%d-id%d", (*pOri).N, (*pOri).id);
    else if ((*pIn).input == 0 && ((*pOri).id == -1))
      sprintf ((*pIn).body, "n%d-id0", (*pOri).N);
    else if ((*pIn).input == 1)
      ut_string_body ((*pIn).oin, (*pIn).body);
    else if ((*pIn).input >= 2 && (*pIn).input <= 8)
      ut_string_body ((*pIn).load, (*pIn).body);
    else
      ut_error_reportbug ();
  }

  (*pIn).ori = ut_string_addextension ((*pIn).body, ".ori");
  (*pIn).col = ut_string_addextension ((*pIn).body, ".col");

  return;
}
