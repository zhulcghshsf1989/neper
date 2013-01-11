/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nem_input.h"

void
nem_input (struct IN *pIn, struct MESHPARA *pMeshPara, int fargc, char
    **fargv, int argc, char **argv)
{
  ut_print_message (0, 2, "Reading arguments ...\n");
  nem_input_treatargs (fargc, fargv, argc, argv, pIn, pMeshPara);

  return;
}

void
nem_in_set_zero (struct IN* pIn)
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
  (*pIn).tess         = NULL;
  (*pIn).vox          = NULL;
  (*pIn).stn          = NULL;
  (*pIn).ste          = NULL;
  (*pIn).stelset      = NULL;
  (*pIn).loadmesh     = NULL;
  (*pIn).msh          = NULL;
  (*pIn).abq          = NULL;
  (*pIn).geof         = NULL;
  (*pIn).fepx1         = NULL;
  (*pIn).fepx2         = NULL;
  (*pIn).fepx3         = NULL;
  (*pIn).fepx4         = NULL;
  (*pIn).fepx5         = NULL;

  (*pIn).mesh3doptiexpr = NULL;
  (*pIn).mesh3doptidisexpr = NULL;
  // (*pIn).mesh3doptisizeexpr = NULL;
  (*pIn).outdim = NULL;

  return;
}

void
nem_in_free (struct IN In)
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
  ut_free_1d_char (In.tess);
  ut_free_1d_char (In.vox);
  ut_free_1d_char (In.stn);
  ut_free_1d_char (In.ste);
  ut_free_1d_char (In.stelset);
  ut_free_1d_char (In.msh);
  ut_free_1d_char (In.abq);
  ut_free_1d_char (In.geof);
  ut_free_1d_char (In.fepx1);
  ut_free_1d_char (In.fepx2);
  ut_free_1d_char (In.fepx3);
  ut_free_1d_char (In.fepx4);
  ut_free_1d_char (In.fepx5);
  ut_free_1d_char (In.outdim);
  ut_free_1d_char (In.mesh3doptiexpr);
  ut_free_1d_char (In.mesh3doptidisexpr);

  return;
}

void
nem_meshpara_set_zero (struct MESHPARA* pMeshPara)
{
  (*pMeshPara).dbound = NULL;
  (*pMeshPara).cl3 = NULL;
  (*pMeshPara).rcl3 = NULL;

  return;
}

void
nem_meshpara_free (struct MESHPARA MeshPara)
{
  ut_free_1d_char (MeshPara.dbound);
  ut_free_1d (MeshPara.cl3);
  ut_free_1d (MeshPara.rcl3);

  return;
}
