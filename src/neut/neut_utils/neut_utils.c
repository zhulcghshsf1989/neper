/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"neut_utils.h"

int
neut_utils_nset_expand (char *nset, char **pexpandnset)
{
  (*pexpandnset) = ut_alloc_1d_char (1000);

  sprintf ((*pexpandnset), "%s", nset);

  ut_string_fnrs ((*pexpandnset), "faces", "x0,x1,y0,y1,z0,z1", 1000);
  ut_string_fnrs ((*pexpandnset), "edges",
		  "x0y0,x0y1,x1y0,x1y1,y0z0,y0z1,y1z0,y1z1,x0z0,x0z1,x1z0,x1z1",
		  1000);
  ut_string_fnrs ((*pexpandnset), "vertices",
		  "x0y0z0,x0y1z0,x1y0z0,x1y1z0,x0y0z1,x0y1z1,x1y0z1,x1y1z1",
		  1000);
  ut_string_fnrs ((*pexpandnset), "facebodies",
		  "x0body,x1body,y0body,y1body,z0body,z1body", 1000);
  ut_string_fnrs ((*pexpandnset), "edgebodies",
		  "x0y0body,x0y1body,x1y0body,x1y1body,y0z0body,y0z1body,y1z0body,y1z1body,x0z0body,x0z1body,x1z0body,x1z1body",
		  1000);
  ut_string_fnrs ((*pexpandnset), "vertices",
		  "x0y0z0,x0y1z0,x1y0z0,x1y1z0,x0y0z1,x0y1z1,x1y0z1,x1y1z1",
		  1000);

  (*pexpandnset) = ut_realloc_1d_char ((*pexpandnset), strlen ((*pexpandnset)) + 1);

  return 0;
}
