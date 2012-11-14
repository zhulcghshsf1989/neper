/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_fm.h"

void
InputData_fm (struct IN *pIn, struct GEOPARA *pGeoPara, int fargc, char
    **fargv, int argc, char **argv)
{
  ut_print_message (0, 2, "Reading arguments ...\n");
  TreatArg_fm (fargc, fargv, argc, argv, pIn, pGeoPara);

  return;
}

void
nefm_in_set_zero (struct IN* pIn)
{
  (*pIn).body         = NULL;
  (*pIn).gmsh         = NULL;
  (*pIn).format       = NULL;
  (*pIn).mesh2dalgo   = NULL;
  (*pIn).mesh3dalgo   = NULL;
  (*pIn).meshpoly     = NULL;
  (*pIn).meshface     = NULL;
  (*pIn).nset         = NULL;
  (*pIn).faset        = NULL;
  (*pIn).partmethod   = NULL;
  (*pIn).partarchfile = NULL;
  (*pIn).npart        = NULL;
  (*pIn).epart        = NULL;
  (*pIn).ingeo        = NULL;
  (*pIn).stn          = NULL;
  (*pIn).ste          = NULL;
  (*pIn).stelset      = NULL;
  (*pIn).loadmesh     = NULL;
  (*pIn).msh          = NULL;
  (*pIn).abq          = NULL;
  (*pIn).geof         = NULL;
  (*pIn).stenode      = NULL;
  (*pIn).steele       = NULL;
  (*pIn).scm          = NULL;
  (*pIn).scg          = NULL;
  (*pIn).fev1         = NULL;
  (*pIn).fev2         = NULL;
  (*pIn).fev3         = NULL;
  (*pIn).fev4         = NULL;
  (*pIn).fev5         = NULL;

  (*pIn).mesh3doptiexpr = NULL;
  (*pIn).mesh3doptidisexpr = NULL;
  // (*pIn).mesh3doptisizeexpr = NULL;
  (*pIn).rem1 = NULL;
  (*pIn).outdim = NULL;

  return;
}

void
nefm_in_free (struct IN In)
{
  ut_free_1d_char (In.body);
  ut_free_1d_char (In.gmsh);
  ut_free_1d_char (In.format);
  ut_free_1d_char (In.mesh2dalgo);
  ut_free_1d_char (In.mesh3dalgo);
  ut_free_1d_char (In.nset);
  ut_free_1d_char (In.faset);
  ut_free_1d_char (In.partmethod);
  ut_free_1d_char (In.partarchfile);
  ut_free_1d_char (In.npart);
  ut_free_1d_char (In.epart);
  ut_free_1d_char (In.ingeo);
  ut_free_1d_char (In.stn);
  ut_free_1d_char (In.ste);
  ut_free_1d_char (In.stelset);
  ut_free_1d_char (In.msh);
  ut_free_1d_char (In.abq);
  ut_free_1d_char (In.geof);
  ut_free_1d_char (In.stenode);
  ut_free_1d_char (In.steele);
  ut_free_1d_char (In.scm);
  ut_free_1d_char (In.scg);
  ut_free_1d_char (In.fev1);
  ut_free_1d_char (In.fev2);
  ut_free_1d_char (In.fev3);
  ut_free_1d_char (In.fev4);
  ut_free_1d_char (In.fev5);
  ut_free_1d_char (In.rem1);
  ut_free_1d_char (In.outdim);
  ut_free_1d_char (In.mesh3doptiexpr);
  ut_free_1d_char (In.mesh3doptidisexpr);

  return;
}

void
nefm_geopara_set_zero (struct GEOPARA* pGeoPara)
{
  (*pGeoPara).dbound = NULL;
  (*pGeoPara).cl3 = NULL;
  (*pGeoPara).rcl3 = NULL;

  return;
}

void
nefm_geopara_free (struct GEOPARA GeoPara)
{
  ut_free_1d_char (GeoPara.dbound);
  ut_free_1d (GeoPara.cl3);
  ut_free_1d (GeoPara.rcl3);

  return;
}
