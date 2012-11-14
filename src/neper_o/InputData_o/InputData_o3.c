/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_o.h"

/* SetDefaultOptions set the options to their default values */
void
SetDefaultOptions_o (struct ORI *pOri, struct IN *pIn)
{
  (*pOri).N = 0;
  (*pOri).id = -1;

  (*pIn).oin = NULL;
  (*pOri).rngid = NULL;

  (*pIn).body = NULL;
  (*pIn).ori  = NULL;
  (*pIn).verbosity = 0;
  (*pIn).descriptor = ut_alloc_1d_char (100);
  sprintf ((*pIn).descriptor, "e");
  (*pIn).crysym = ut_alloc_1d_char (100);
  sprintf ((*pIn).crysym, "triclinic");
  (*pIn).format = ut_alloc_1d_char (100);
  sprintf ((*pIn).format, "plain");

  (*pIn).input = -1;

  (*pIn).colourspec = NULL;

  return;
}

void
SetOptions_o (struct ORI *pOri, struct IN *pIn,
	      int argc, char **argv)
{
  int i, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);

  /* This is the possible argument list. */
  ArgQty = 0;
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-n");
  sprintf (ArgList[++ArgQty], "-id");
  sprintf (ArgList[++ArgQty], "-v");
  sprintf (ArgList[++ArgQty], "-crysym");
  sprintf (ArgList[++ArgQty], "-descriptor");
  sprintf (ArgList[++ArgQty], "-format");
  sprintf (ArgList[++ArgQty], "-colour");
  sprintf (ArgList[++ArgQty], "-load");

  for (i = 1; i <= argc - 1; i++)
  {
    if (argv[i][0] != '-')
    {
      if ((*pIn).input == -1)
      {
	(*pIn).oin = ut_alloc_1d_char (strlen (argv[i]));
	(*pIn).oin = argv[i];
	(*pIn).input = 1;
      }
      else
	ut_arg_badarg ();
    }
    else
    {
      /* Searching option name (string completion stuff) */
      Res = ut_string_comp (argv[i], ArgList, ArgQty, Arg);
      if (Res == 1)
      {
	ut_print_lineheader (2);
	printf ("Several possibilities for option `%s'.\n", argv[i]);
	ut_arg_badarg ();
      }
      else if (Res == -1)
      {
	ut_print_lineheader (2);
	printf ("Unknown option `%s'.\n", argv[i]);
	ut_arg_badarg ();
      }

      if (strcmp (Arg, "-o") == 0 && i < argc - 1)
      {
	(*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
	ut_string_body ((*pIn).body, (*pIn).body);
      }
      else if (strcmp (Arg, "-n") == 0 && i < argc - 1)
      {
	(*pOri).N = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
	(*pIn).input = 0;
      }
      else if (strcmp (Arg, "-id") == 0 && i < argc - 1)
	(*pOri).id = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      else if (strcmp (Arg, "-v") == 0 && i < argc - 1)
	(*pIn).verbosity = ut_arg_nextasint (argv, &i, Arg, 0, 1);
      else if (strcmp (Arg, "-crysym") == 0 && i < argc - 1)
	(*pIn).crysym = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-descriptor") == 0 && i < argc - 1)
	(*pIn).descriptor = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-format") == 0 && i < argc - 1)
	(*pIn).format = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-colour") == 0 && i < argc - 1)
	(*pIn).colourspec = ut_arg_nextaschar (argv, &i, Arg);
      else if (strcmp (Arg, "-load") == 0 && i < argc - 2)
      {
	char* load_type = ut_arg_nextaschar (argv, &i, Arg);
	if (strcmp (load_type, "e") == 0)
	  (*pIn).input = 2;
	else if (strcmp (load_type, "ek") == 0)
	  (*pIn).input = 3;
	else if (strcmp (load_type, "er") == 0)
	  (*pIn).input = 4;
	else if (strcmp (load_type, "g") == 0)
	  (*pIn).input = 5;
	else if (strcmp (load_type, "rtheta") == 0)
	  (*pIn).input = 6;
	else if (strcmp (load_type, "R") == 0)
	  (*pIn).input = 7;
	else if (strcmp (load_type, "q") == 0)
	  (*pIn).input = 8;

	(*pIn).load = ut_arg_nextaschar (argv, &i, Arg);
	ut_free_1d_char (load_type);
      }
      else
      {
	ut_arg_badarg ();
	abort ();
      }
    }
  }

  ut_free_2d_char (ArgList, 101);
  ut_free_1d_char (Arg);

  return;
}
