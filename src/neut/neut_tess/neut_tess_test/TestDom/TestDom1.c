/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestDom.h"

int
neut_tess_test_dom (struct TESS Tess, int verbosity)
{
  if (neut_tess_test_dom_def      (Tess, verbosity) != 0
   || neut_tess_test_dom_tessver  (Tess, verbosity) != 0
   || neut_tess_test_dom_tessedge (Tess, verbosity) != 0
   || neut_tess_test_dom_tessface (Tess, verbosity) != 0)
    return -1;
  else
    return 0;
}
