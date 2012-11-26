/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestDom.h"

int
neut_geo_test_dom (struct GEO Geo, int verbosity)
{
  if (neut_geo_test_dom_def      (Geo, verbosity) != 0
   || neut_geo_test_dom_tessver  (Geo, verbosity) != 0
   || neut_geo_test_dom_tessedge (Geo, verbosity) != 0
   || neut_geo_test_dom_tessface (Geo, verbosity) != 0)
    return -1;
  else
    return 0;
}
