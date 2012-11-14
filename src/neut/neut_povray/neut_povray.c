/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_povray.h"

int
neut_povray_check (void)
{
  int qty;

  system ("which povray 2> /dev/null > neper.tmp");
  qty = ut_file_nbwords ("neper.tmp");
  remove ("neper.tmp");

  if (qty == 0)
    return -1;
  else
    return 0;
}

int
neut_povray_check_error (void)
{
  if (neut_povray_check () != 0)
  {
    ut_print_messagewnc (2, 72, "Povray not found! Make sure it is available at the command line then run Neper again.\n");
    abort ();
  }

  return 0;
}
