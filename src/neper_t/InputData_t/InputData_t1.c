/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

void
InputData_t (struct IN *pIn, struct GERMSET *pGermSet,
	     int fargc, char **fargv, int argc, char **argv)
{
  TreatArg_t (fargc, fargv, argc, argv, pIn, pGermSet);

  return;
}

void
net_in_set_zero (struct IN* pIn)
{
  (*pIn).input = 0;
  (*pIn).checktess = 0;

  (*pIn).domain = NULL;
  (*pIn).F = NULL;

  (*pIn).verbosity = 0;

  (*pIn).format = NULL;
  (*pIn).body   = NULL;
  (*pIn).load   = NULL;
  (*pIn).geo   = NULL;
  (*pIn).gmshgeo   = NULL;
  (*pIn).ply   = NULL;
  (*pIn).dec   = NULL;
  (*pIn).oin  = NULL;
  (*pIn).mast  = NULL;
  (*pIn).asy   = NULL;
  (*pIn).stt3   = NULL;
  (*pIn).stt0   = NULL;
  (*pIn).neigh   = NULL;
  (*pIn).debug   = NULL;
  
  (*pIn).printstattess = ut_alloc_1d_char (5);
  strcpy ((*pIn).printstattess, "none");
  (*pIn).sorttess_qty = 0;
  (*pIn).sorttess = NULL;
  (*pIn).printneighbour = 0;
  (*pIn).printpointpoly = 0;
  (*pIn).point = NULL;
  (*pIn).polyid = NULL;
  (*pIn).libpath = NULL;

  (*pIn).centroid = 0;
  (*pIn).centroiditermax = 0;
  (*pIn).centroidfact = 0;
  (*pIn).centroidconv = 0;

  return;
}

void
net_in_free (struct IN In)
{
  ut_free_1d_char (In.domain);
  ut_free_1d (In.F);
  ut_free_1d_char (In.format);
  ut_free_1d_char (In.body);
  ut_free_1d_char (In.load);
  ut_free_1d_char (In.geo);
  ut_free_1d_char (In.gmshgeo);
  ut_free_1d_char (In.ply);
  ut_free_1d_char (In.dec);
  ut_free_1d_char (In.oin);
  ut_free_1d_char (In.mast);
  ut_free_1d_char (In.asy);
  ut_free_1d_char (In.stt3);
  ut_free_1d_char (In.stt0);
  ut_free_1d_char (In.debug);
  ut_free_2d_char (In.sorttess, In.sorttess_qty);
  ut_free_1d_char (In.neigh);
  ut_free_1d      (In.domainparms);
  ut_free_1d_char (In.point);
  ut_free_1d_char (In.polyid);

  return;
}
