/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

int
SetOptions_t_inputdata (struct GERMSET *pGermSet,
			int argc, char **argv, int* pi, char* Arg, int* pinput)
{
  int status = 0;

  if (strcmp (Arg, "-n") == 0 && (*pi) < argc - 1)
  {
    if ((*pinput) == -1)
    {
      (*pinput) = 0;
      (*pGermSet).N = ut_arg_nextasint (argv, pi, Arg, 1, INT_MAX);
      (*pGermSet).NDensity = (*pGermSet).N;
      
      if ((strcmp ((*pGermSet).morpho, "cube"  ) == 0)
       || (strcmp ((*pGermSet).morpho, "dodeca") == 0)
       || (strcmp ((*pGermSet).morpho, "tocta" ) == 0))
      {
	(*pGermSet).N1d = (*pGermSet).N;
	(*pGermSet).N   = -1;
	(*pinput) = 2;
      }
    }
    else
      ut_arg_badarg ();
  }
  else if (strcmp (Arg, "-id") == 0 && (*pi) < argc - 1)
  {
    if ((*pGermSet).Id == 0)
    {
      ut_print_message (2, 0,
			"Functions `-id' and `-centrecoo' are mutually exclusive!");
      abort ();
    }
    (*pGermSet).Id = ut_arg_nextasint (argv, pi, Arg, 1, INT_MAX);
  }
  else if (strcmp (Arg, "-centrecoo") == 0
	|| strcmp (Arg, "-centercoo") == 0
	|| strcmp (Arg,   "-gcoo"   ) == 0)
  {
    if ((*pGermSet).Id > 0)
    {
      ut_print_message (2, 0,
			"Options `-id' and `-centrecoo' are mutually exclusive!");
      abort ();
    }
    (*pGermSet).Id = 0;
    char* tmp = ut_arg_nextaschar (argv, pi, Arg);
    (*pGermSet).morpho = ut_realloc_1d_char ((*pGermSet).morpho, strlen (tmp) + 2);
    sprintf ((*pGermSet).morpho, "@%s", tmp);
    ut_free_1d_char (tmp);
  }
  else if (strcmp (Arg, "-morpho") == 0)
  {
    if ((*pGermSet).morpho != NULL)
      ut_free_1d_char ((*pGermSet).morpho);
    (*pGermSet).morpho = ut_arg_nextaschar (argv, pi, Arg);

    if ((*pGermSet).N > 0)
      if ((! strcmp ((*pGermSet).morpho, "cube"  ))
       || (! strcmp ((*pGermSet).morpho, "dodeca"))
       || (! strcmp ((*pGermSet).morpho, "tocta" )))
      {
	(*pGermSet).N1d = (*pGermSet).N;
	(*pGermSet).N   = -1;
	(*pinput)    =  2;
      }
  }
  else if (strcmp (Arg, "-ttype") == 0 && (*pi) < argc - 1)
  {
    ut_free_1d_char ((*pGermSet).ttype);
    (*pGermSet).ttype = ut_arg_nextaschar (argv, pi, Arg);
  }
  else if (strcmp (Arg, "-regular") == 0 && (*pi) < argc - 2)
  {
    ut_print_messagewnc (1, 72, "Option -regular is deprecated and will be removed from future neper versions.  Use -morpho instead.");
    if ((*pGermSet).morpho != NULL)
      ut_free_1d_char ((*pGermSet).morpho);
    (*pGermSet).morpho = ut_arg_nextaschar (argv, pi, Arg);
    (*pGermSet).N1d = ut_arg_nextasint (argv, pi, Arg, 0, INT_MAX);
    (*pinput) = 2;
  }
  else
    status = -1;

  return status;
}
