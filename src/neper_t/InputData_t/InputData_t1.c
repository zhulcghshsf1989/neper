/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_t.h"

void
InputData_t (struct IN *pIn, struct GEOPARA* pGeoPara, char** paction, 
	     int fargc, char **fargv, int argc, char **argv)
{
  int tess_out, vox_out;
  int tessellate, voxelize, tess_voxelize;

  TreatArg_t (fargc, fargv, argc, argv, pIn, pGeoPara);

  tess_out = 0;
  vox_out  = 0;
  if (ut_string_inlist ((*pIn).format, ',', "tess") == 1
   || ut_string_inlist ((*pIn).format, ',', "geo" ) == 1
   || ut_string_inlist ((*pIn).format, ',', "ply" ) == 1)
    tess_out = 1;
  if (ut_string_inlist ((*pIn).format, ',', "vox") == 1)
    vox_out  = 1;

  if (tess_out == 1 && strcmp ((*pIn).ttype, "standard") != 0)
  {
    ut_print_message (2, 2, "tess-type output not available with ttype not `standard'.\n");
    abort ();
  }

  tessellate    = 0;
  voxelize      = 0;
  tess_voxelize = 0;

  // Input is -n or -n_reg and tocta
  if (! strcmp ((*pIn).input, "n")
  || (! strcmp ((*pIn).input, "n_reg") && ! strcmp ((*pIn).morpho, "tocta")))
  {
    // Asked for a tess-type output?
    if (tess_out == 1)
    {
      tessellate = 1;
      // + asked for a voxel-type output?
      if (vox_out == 1)
	tess_voxelize = 1;
    }

    // Only asked for a voxel-type output?
    else if (vox_out == 1)
    {
      if ((*pIn).centroid == 1)
      {
	tessellate = 1;
	tess_voxelize = 1;
      }

      /*
      // Regularization?
      if ((*pIn).maxff > 0)
      {
	tessellate = 1;
	tess_voxelize = 1;
      }
      else
      */
      else
	voxelize = 1;
    }
  }

  // Input is -n_reg, but not tocta
  else if (! strcmp ((*pIn).input, "n_reg") && strcmp ((*pIn).morpho, "tocta") != 0)
  {
    // Asked for a tess-type output?
    if (tess_out == 1)
    {
      ut_print_message (2, 0, "tess-type output cannot be obtained for morpho=%s\n", (*pIn).morpho);
      abort ();
    }

    else if (vox_out == 1)
      voxelize = 1;
  }

  // Input is a tess file
  else if (! strcmp ((*pIn).input, "tess"))
  {
    // Asked for a voxel-type output?
    if (vox_out == 1)
      tess_voxelize = 1;
  }

  (*paction) = ut_alloc_1d_char (30);
  if (tessellate)
  {
    strcpy ((*paction), "tessellate");
    if (tess_voxelize)
      (*paction) = strcat ((*paction), ",tess_voxelize");
  }
  else if (! strcmp ((*pIn).input, "tess"))
  {
    if (tess_voxelize)
      (*paction) = strcat ((*paction), "tess_voxelize");
  }
  else if (voxelize)
    strcpy ((*paction), "voxelize");

  return;
}

void
net_in_set_zero (struct IN* pIn)
{
  (*pIn).input = 0;
  (*pIn).checktess = 0;

  (*pIn).domain = NULL;
  (*pIn).scale = NULL;

  (*pIn).format = NULL;
  (*pIn).voxformat = NULL;
  (*pIn).body   = NULL;
  (*pIn).load   = NULL;
  (*pIn).tess   = NULL;
  (*pIn).vox    = NULL;
  (*pIn).geo    = NULL;
  (*pIn).ply   = NULL;
  (*pIn).debug   = NULL;
  
  (*pIn).printstattess = ut_alloc_1d_char (5);
  strcpy ((*pIn).printstattess, "none");
  (*pIn).sorttess_qty = 0;
  (*pIn).sorttess = NULL;
  (*pIn).printpointpoly = 0;
  (*pIn).point = NULL;
  (*pIn).polyid = NULL;

  (*pIn).centroid = 0;
  (*pIn).centroiditermax = 0;
  (*pIn).centroidfact = 0;
  (*pIn).centroidconv = 0;
  
  (*pIn).voxsizetype = 0;
  (*pIn).voxsize = 0;
  (*pIn).voxsize3 = NULL;

  return;
}

void
net_in_free (struct IN In)
{
  ut_free_1d_char (In.domain);
  ut_free_1d      (In.scale);
  ut_free_1d_char (In.format);
  ut_free_1d_char (In.voxformat);
  ut_free_1d_char (In.body);
  ut_free_1d_char (In.load);
  ut_free_1d_char (In.tess);
  ut_free_1d_char (In.vox);
  ut_free_1d_char (In.geo);
  ut_free_1d_char (In.ply);
  ut_free_1d_char (In.debug);
  ut_free_2d_char (In.sorttess, In.sorttess_qty);
  ut_free_1d      (In.domainparms);
  ut_free_1d_char (In.point);
  ut_free_1d_char (In.polyid);
  ut_free_1d_int  (In.voxsize3);

  return;
}
