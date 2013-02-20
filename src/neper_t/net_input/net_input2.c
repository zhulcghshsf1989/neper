/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input.h"

void
net_input_treatargs (int fargc, char **fargv, int argc, char **argv,
            struct IN* pIn, struct REG* pReg)
{
  int i;

  net_input_options_default (pIn, pReg);

  /* Reading options */

  net_input_options_set (pIn, pReg, fargc, fargv);
  net_input_options_set (pIn, pReg, argc, argv);

  // Processing nstring
  int argqty, argqty2;
  char** args = NULL;
  char** args2 = NULL;

  if ((*pIn).nstring != NULL)
  {
    ut_string_separate ((*pIn).nstring, ':', &args, &argqty);
    (*pIn).levelqty = argqty;
    (*pIn).n = ut_alloc_1d_pchar ((*pIn).levelqty + 1);
    (*pIn).n[0] = ut_alloc_1d_char (2);
    strcpy ((*pIn).n[0], "1");
    for (i = 1; i <= (*pIn).levelqty; i++)
    {
      (*pIn).n[i] = ut_alloc_1d_char (strlen (args[i - 1]) + 1);
      strcpy ((*pIn).n[i], args[i - 1]);
    }
    ut_free_2d_char (args, (*pIn).levelqty);
    args = NULL;
  }
  else
    (*pIn).levelqty = 0;

  if ((*pIn).levelqty > 0)
  {
    // Processing idstring
    (*pIn).id = ut_alloc_1d_int ((*pIn).levelqty + 1);
    ut_array_1d_int_set ((*pIn).id, (*pIn).levelqty + 1, -1);

    argqty = 0;
    if ((*pIn).idstring != NULL)
    {
      ut_string_separate ((*pIn).idstring, ':', &args, &argqty);
      if (argqty > (*pIn).levelqty)
      {
	ut_print_message (2, 0, "The number of levels in option `-id' exceeds the number of levels in option `-n'.");
	abort ();
      }

      for (i = 1; i <= argqty; i++)
	sscanf (args[i - 1], "%d", &((*pIn).id[i]));
      ut_free_2d_char (args, argqty);
    }

    // Processing morphostring
    (*pIn).morpho = ut_alloc_1d_pchar ((*pIn).levelqty + 1);
    if ((*pIn).morphostring != NULL)
    {
      ut_string_separate ((*pIn).morphostring, ':', &args, &argqty);
      if (argqty > (*pIn).levelqty)
      {
	ut_print_message (2, 0, "The number of levels in option `-morpho' exceeds the number of levels in option `-n'.");
	abort ();
      }

      for (i = 1; i <= argqty; i++)
      {
	(*pIn).morpho[i] = ut_alloc_1d_char (strlen (args[i - 1]) + 1);
	strcpy ((*pIn).morpho[i], args[i - 1]);
	if (i > 1 && ! strcmp ((*pIn).morpho[i], "tocta"))
	{
	  ut_print_message (2, 0, "`-morpho tocta' is only available at level 1.\n");
	  abort ();
	}
      }
      ut_free_2d_char (args, argqty);
      args = NULL;

      for (i = argqty + 1; i <= (*pIn).levelqty; i++)
      {
	(*pIn).morpho[i] = ut_alloc_1d_char (8);
	strcpy ((*pIn).morpho[i], "poisson");
      }
    }

    // Processing centroidstring
    (*pIn).centroid = ut_alloc_1d_int ((*pIn).levelqty + 1);

    if ((*pIn).centroidstring != NULL)
    {
      ut_string_separate ((*pIn).centroidstring, ':', &args, &argqty);
      for (i = 1; i <= argqty; i++)
	sscanf (args[i - 1], "%d", &((*pIn).centroid[i]));
      ut_free_2d_char (args, argqty);
    }

    // Processing regstring
    (*pReg).reg = ut_alloc_1d_int ((*pIn).levelqty + 1);

    if ((*pReg).regstring != NULL)
    {
      ut_string_separate ((*pReg).regstring, ':', &args, &argqty);
      for (i = 1; i <= argqty; i++)
	sscanf (args[i - 1], "%d", &((*pReg).reg[i]));
      ut_free_2d_char (args, argqty);
    }

    // Testing consistenct between morpho = lamellae and n, which must
    // start by lamwidth=
    if ((*pIn).morphostring != NULL)
    {
      ut_string_separate ((*pIn).morphostring, ':', &args, &argqty);
      ut_string_separate ((*pIn).nstring, ':', &args2, &argqty2);
      for (i = 0; i < argqty; i++)
	if (! strcmp (args[i], "lamellae"))
	  if (strncmp (args2[i], "d=", 2) != 0)
	    ut_print_message (2, 0, "With `-morpho lamellae', you must use `-n d='.\n");

      ut_free_2d_char (args, argqty);
      ut_free_2d_char (args2, argqty2);
    }
  }


  /* Testing options */
  if ((*pIn).input == NULL)
  {
    ut_print_message (2, 0, "missing input data.");
    abort ();
  }

  if (! strcmp ((*pIn).input, "n") && (*pIn).id == NULL)
    ut_print_messagewnc (1, 72,
		"The identifier (-id) has not been specified; considering a random value ...");

  if ((*pIn).idstring != NULL
  && ((strncmp ((*pIn).morphostring, "cube"  , 4) == 0)
   || (strncmp ((*pIn).morphostring, "dodeca", 6) == 0)
   || (strncmp ((*pIn).morphostring, "tocta" , 5) == 0)))
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
      if ((*pIn).idstring != NULL)
      {
	sprintf ((*pIn).body, "n%s-id", (*pIn).nstring);
	for (i = 1; i <= (*pIn).levelqty; i++)
	{
	  if ((*pIn).id[i] > 0)
	    sprintf ((*pIn).body, "%s%d", (*pIn).body, (*pIn).id[i]);
	  else
	    (*pIn).body = strcat ((*pIn).body, "rand");

	  if (i < (*pIn).levelqty)
	    (*pIn).body = strcat ((*pIn).body, ":");
	}
      }
      else
      {
	if (! strcmp ((*pIn).morpho[1], "poisson"))
	  sprintf ((*pIn).body, "n%s-rand", (*pIn).nstring);
	else if ((*pIn).morpho[1][0] != '@')
	  sprintf ((*pIn).body, "n%s-%s", (*pIn).nstring, (*pIn).morpho[1]);
	else
	  sprintf ((*pIn).body, "n%s-coo", (*pIn).nstring);
      }
    }
    else if (! strcmp ((*pIn).input, "tess")
          || ! strcmp ((*pIn).input, "vox"))
    {
      (*pIn).body = ut_alloc_1d_char (strlen ((*pIn).load));
      ut_string_body ((*pIn).load, (*pIn).body);
    }
    else if (! strcmp ((*pIn).input, "n_reg"))
      sprintf ((*pIn).body, "n%s-%s", (*pIn).nstring, (*pIn).morpho[1]);
    else
      abort ();
  }

  (*pIn).tess = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).geo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).vox = ut_string_addextension ((*pIn).body, ".vox");
  (*pIn).raw = ut_string_addextension ((*pIn).body, ".raw");
  (*pIn).ply = ut_string_addextension ((*pIn).body, ".ply");
  (*pIn).dec = ut_string_addextension ((*pIn).body, ".3dec");
  (*pIn).debug = ut_string_addextension ((*pIn).body, ".debug");

  if ((*pIn).printpointpoly == 1)
    (*pIn).polyid = ut_string_addextension ((*pIn).point, ".polyid");
  else if ((*pIn).printpointpoly == 2)
    (*pIn).polyid = ut_string_paste ((*pIn).point, "-centre.polyid");

  return;
}
