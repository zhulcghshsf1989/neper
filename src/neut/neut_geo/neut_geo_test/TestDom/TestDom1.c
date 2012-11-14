/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"TestDom.h"

int
neut_geo_test_dom (struct GEO Geo)
{
  int status;

  status = neut_geo_test_dom_def (Geo);
  if (status != 0)
  {
    printf ("neut_geo_test_dom failed (returned %d)\n", status);
    return status;
  }

  status = neut_geo_test_dom_tessver (Geo);
  if (status != 0)
  {
    printf ("neut_geo_test_dom_tessver failed (returned %d)\n", status);
    return status;
  }

  status = neut_geo_test_dom_tessedge (Geo);
  if (status != 0)
  {
    printf ("neut_geo_test_dom_tessedge failed (returned %d)\n", status);
    abort ();
    return status;
  }

  status = neut_geo_test_dom_tessface (Geo);
  if (status != 0)
  {
    printf ("neut_geo_test_dom_tessface failed (returned %d)\n", status);
    return status;
  }

  return 0;
}
