/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_mm.h"

/* SetDefaultOptions set the options to their default values */
void
SetDefaultOptions_mm (struct IN *pIn)
{
  (*pIn).format = ut_alloc_1d_char (100);
  sprintf ((*pIn).format, "msh");

  (*pIn).domain = ut_alloc_1d_char (10);
  sprintf ((*pIn).domain, "cube");
  (*pIn).domainparms = ut_alloc_1d (3);
  (*pIn).domainparms[0] = 1;
  (*pIn).domainparms[1] = 1;
  (*pIn).domainparms[2] = 1;
  (*pIn).domainparms2 = NULL;

  (*pIn).ingeo = NULL;
  (*pIn).body = NULL;
  (*pIn).geo = NULL;
  (*pIn).gmshgeo = NULL;
  (*pIn).geof = NULL;
  (*pIn).msh = NULL;
  (*pIn).centrecoo = NULL;

  (*pIn).input = NULL;
  (*pIn).nset = ut_alloc_1d_char (1000);
  sprintf ((*pIn).nset, "faces");
  (*pIn).faset = ut_alloc_1d_char (1000);
  (*pIn).faset[0] = '\0';

  (*pIn).msizetype = 1;
  (*pIn).msize = 20;
  (*pIn).msize3 = ut_alloc_1d_int (3);
  (*pIn).msize3[0] = 20;
  (*pIn).msize3[1] = 20;
  (*pIn).msize3[2] = 20;
  (*pIn).morder = 1;

  (*pIn).mesh = 0;
  (*pIn).loadmesh = NULL;

  (*pIn).outdim = ut_alloc_1d_char(100);
  sprintf ((*pIn).outdim, "3");
  (*pIn).meshpoly = NULL;
  (*pIn).singnodedup = 0;
  (*pIn).cleaning = 0;

  return;
}

void
SetOptions_mm (struct IN *pIn, int argc, char **argv)
{
  int i, ArgQty, Res;
  char **ArgList = ut_alloc_2d_char (101, 101);
  char *Arg = ut_alloc_1d_char (101);

  /* This is the possible argument list. */
  ArgQty = 0;
  // Input data --------------------------------------------------------
  sprintf (ArgList[++ArgQty], "-n");
  sprintf (ArgList[++ArgQty], "-id");
  sprintf (ArgList[++ArgQty], "-morpho");
  sprintf (ArgList[++ArgQty], "-centrecoo");
  sprintf (ArgList[++ArgQty], "-centercoo");
  sprintf (ArgList[++ArgQty], "-gcoo");
  sprintf (ArgList[++ArgQty], "-domain");
  // General options ---------------------------------------------------
  sprintf (ArgList[++ArgQty], "-o");
  sprintf (ArgList[++ArgQty], "-ttype");
  sprintf (ArgList[++ArgQty], "-nset");
  sprintf (ArgList[++ArgQty], "-msize");
  sprintf (ArgList[++ArgQty], "-morder");
  sprintf (ArgList[++ArgQty], "-order");
  sprintf (ArgList[++ArgQty], "-v");
  sprintf (ArgList[++ArgQty], "-faset");
  sprintf (ArgList[++ArgQty], "-msize3");
  sprintf (ArgList[++ArgQty], "-format");
  sprintf (ArgList[++ArgQty], "-loadmesh");
  sprintf (ArgList[++ArgQty], "-outdim");
  sprintf (ArgList[++ArgQty], "-meshpoly");
  sprintf (ArgList[++ArgQty], "-singnodedup");
  sprintf (ArgList[++ArgQty], "-cleaning");

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
    {
      (*pIn).input = ut_alloc_1d_char (5);
      sprintf ((*pIn).input, "n");
      (*pIn).ingeo = ut_alloc_1d_char (strlen (argv[i]) + 1);
      sprintf ((*pIn).ingeo, "%s", argv[i]);
      (*pIn).mesh = 1;
      continue;
    }
    
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
    
    /*---------------------------------------------------------------------- 
     * input data */

    if (strcmp (Arg, "-o") == 0 && i < argc - 1)
    {
      (*pIn).body = ut_arg_nextaschar (argv, &i, Arg);
      ut_string_body ((*pIn).body, (*pIn).body);
    }
    else if (strcmp (Arg, "-format") == 0 && i < argc - 1)
      (*pIn).format = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-nset") == 0 && i < argc - 1)
    {
      if ((*pIn).nset != NULL)
	ut_free_1d_char ((*pIn).nset);
      (*pIn).nset = ut_arg_nextaschar (argv, &i, Arg);
    }
    else if (strcmp (Arg, "-faset") == 0 && i < argc - 1)
      (*pIn).faset = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-msize") == 0 && i < argc - 1)
    {
      (*pIn).msizetype = 1;
      (*pIn).msize = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
    }
    else if (strcmp (Arg, "-msize3") == 0 && i < argc - 1)
    {
      (*pIn).msizetype = 3;
      (*pIn).msize3[0] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      (*pIn).msize3[1] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
      (*pIn).msize3[2] = ut_arg_nextasint (argv, &i, Arg, 1, INT_MAX);
    }
    else if (strcmp (Arg, "-domain") == 0 && i < argc - 1)
    {
      ut_free_1d_char ((*pIn).domain);
      (*pIn).domain = ut_arg_nextaschar (argv, &i, Arg);
      if (! strcmp ((*pIn).domain, "cube"))
      {
	(*pIn).domainparms[0] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[1] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
	(*pIn).domainparms[2] = ut_arg_nextasreal (argv, &i, Arg, 0, DBL_MAX);
      }
      else
      {
	ut_arg_badarg ();
	abort ();
      }
    }
    else if (strcmp (Arg, "-morder") == 0 && i < argc - 1)
      (*pIn).morder = ut_arg_nextasint (argv, &i, Arg, 1, 2);
    else if (strcmp (Arg, "-order") == 0 && i < argc - 1)
      (*pIn).morder = ut_arg_nextasint (argv, &i, Arg, 1, 2);
    else if (strcmp (Arg, "-loadmesh") == 0 && i < argc - 1)
    {
      (*pIn).loadmesh = ut_arg_nextaschar (argv, &i, Arg);
      (*pIn).input = ut_alloc_1d_char (5);
      strcpy ((*pIn).input, "mesh");
    }
    else if (strcmp (Arg, "-outdim") == 0 && i < argc - 1)
      (*pIn).outdim = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-meshpoly") == 0 && i < argc - 1)
      (*pIn).meshpoly = ut_arg_nextaschar (argv, &i, Arg);
    else if (strcmp (Arg, "-cleaning") == 0 && i < argc - 1)
      (*pIn).cleaning = ut_arg_nextasint (argv, &i, Arg, 0, 2);
    else if (strcmp (Arg, "-singnodedup") == 0 && i < argc - 1)
      (*pIn).singnodedup = ut_arg_nextaslogical (argv, &i, Arg);
    else
    {
      ut_arg_badarg ();
      abort ();
    }
  }

  ut_free_2d_char (ArgList, 101);
  ut_free_1d_char (Arg);

  return;
}
