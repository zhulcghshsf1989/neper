/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ut_option.h"
#include"ut.h"

/* Reading options from optionsfile. */
void
ut_option_read (char *optionsfile, int *pfargc, char **fargv,
		char *pname, char *mname)
{
  char Trash[1000];
  int test;
  FILE* file = NULL;

  if (ut_file_exist (optionsfile) == 0)
  {
    (*pfargc) = 0;
    fargv = NULL;
    return;
  }

  file = ut_file_open (optionsfile, "R");

  /* Skipping data until `neper -S' is reached. */
  while (fscanf (file, "%s", Trash) == 1)
  {
    if (strcmp (Trash, pname) == 0)
    {
      if (fscanf (file, "%s", Trash) == 1)
	if (strcmp (Trash, mname) == 0)
	  break;
    }
  }

  /* Reading options */
  (*pfargc) = 1;
  do
  {
    test = fscanf (file, "%s", fargv[(*pfargc)]);
    (*pfargc)++;
  }
  while (test != -1 && strcmp (fargv[(*pfargc) - 1], pname) != 0);

  (*pfargc)--;			/* the last one is nok. */

  ut_file_close (file, optionsfile, "R");

  return;
}
