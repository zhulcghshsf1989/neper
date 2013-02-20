/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"net_input.h"

void
net_input (struct IN *pIn, struct REG* pReg,
	     int fargc, char **fargv, int argc, char **argv)
{
  int tess_out, vox_out;

  net_input_treatargs (fargc, fargv, argc, argv, pIn, pReg);

  tess_out = 0;
  vox_out  = 0;
  if (ut_string_inlist ((*pIn).format, ',', "tess") == 1
   || ut_string_inlist ((*pIn).format, ',', "geo" ) == 1
   || ut_string_inlist ((*pIn).format, ',', "ply" ) == 1)
    tess_out = 1;
  if (ut_string_inlist ((*pIn).format, ',', "vox") == 1
   || ut_string_inlist ((*pIn).format, ',', "raw") == 1)
    vox_out  = 1;

  // Setting mode
  (*pIn).mode = ut_alloc_1d_char (30);

  if (tess_out == 1)
    strcpy ((*pIn).mode, "tess");
  else if (vox_out == 1)
  {
    // centroid and reg can be done only on tess
    if ((*pIn).centroidstring != NULL || (*pReg).regstring != NULL)
      strcpy ((*pIn).mode, "tess");
    else
      strcpy ((*pIn).mode, "vox");
  }

  if (! strcmp ((*pIn).mode, "tess"))
  {
    if (! strcmp ((*pIn).input, "vox"))
    {
      ut_print_message (2, 0, "tess-type output not available with vox input.");
      abort ();
    }
    else if (strcmp ((*pIn).ttype, "standard") != 0)
    {
      ut_print_message (2, 2, "tess-type output not available with ttype not `standard'.\n");
      abort ();
    }
    else if (! strcmp ((*pIn).input, "n_reg") && strcmp ((*pIn).morpho[1], "tocta") != 0)
    {
      ut_print_message (2, 0, "tess-type output not available for morpho=%s\n", (*pIn).morpho[1]);
      abort ();
    }
  }

  return;
}

void
net_in_set_zero (struct IN* pIn)
{
  (*pIn).nstring = NULL;
  (*pIn).n = NULL;
  (*pIn).idstring = NULL;
  (*pIn).id = NULL;
  (*pIn).morphostring = NULL;
  (*pIn).morpho = NULL;
  (*pIn).ttype = NULL;
  (*pIn).levelqty = 0;

  (*pIn).randomize = 0;
  (*pIn).randomize2 = 0;
  (*pIn).randomizedir = NULL;

  (*pIn).mode = NULL;
  (*pIn).input = 0;

  (*pIn).domain = NULL;
  (*pIn).domainparms = NULL;
  (*pIn).domainparms2 = NULL;
  (*pIn).scale = NULL;

  (*pIn).voxsizetype = 0;
  (*pIn).voxsize = 0;
  (*pIn).voxsize3 = NULL;

  (*pIn).checktess = 0;

  (*pIn).format = NULL;
  (*pIn).voxformat = NULL;
  (*pIn).body   = NULL;
  (*pIn).load   = NULL;
  (*pIn).tess   = NULL;
  (*pIn).vox    = NULL;
  (*pIn).raw    = NULL;
  (*pIn).geo    = NULL;
  (*pIn).ply   = NULL;
  (*pIn).stv   = NULL;
  (*pIn).ste   = NULL;
  (*pIn).stf   = NULL;
  (*pIn).stp   = NULL;
  (*pIn).dec   = NULL;
  (*pIn).debug   = NULL;

  (*pIn).printstattess = ut_alloc_1d_char (5);
  strcpy ((*pIn).printstattess, "none");
  (*pIn).sorttess_qty = 0;
  (*pIn).sorttess = NULL;
  (*pIn).printpointpoly = 0;
  (*pIn).point = NULL;
  (*pIn).polyid = NULL;

  (*pIn).centroidstring = NULL;
  (*pIn).centroid = NULL;
  (*pIn).centroiditermax = 0;
  (*pIn).centroidfact = 0;
  (*pIn).centroidconv = 0;

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
  ut_free_1d_char (In.raw);
  ut_free_1d_char (In.geo);
  ut_free_1d_char (In.ply);
  ut_free_1d_char (In.dec);
  ut_free_1d_char (In.debug);
  ut_free_2d_char (In.sorttess, In.sorttess_qty);
  ut_free_1d      (In.domainparms);
  ut_free_1d_char (In.point);
  ut_free_1d_char (In.polyid);
  ut_free_1d_int  (In.voxsize3);

  ut_free_1d_char (In.nstring);
  ut_free_1d_char (In.centroidstring);

  ut_free_1d_char (In.printstattess);
  ut_free_1d_char (In.input);
  ut_free_1d_int (In.centroid);
  ut_free_1d_char (In.ttype);
  ut_free_1d_char (In.morphostring);
  ut_free_2d_char (In.morpho, In.levelqty + 1);
  ut_free_1d_char (In.randomizedir);
  ut_free_2d_char (In.n, In.levelqty + 1);
  ut_free_1d_char (In.idstring);
  ut_free_1d_int  (In.id);
  ut_free_1d_char (In.mode);

  return;
}

void
net_reg_set_zero (struct REG* pReg)
{
  (*pReg).regstring = NULL;
  (*pReg).reg = NULL;
  (*pReg).mloop = 2;
  (*pReg).maxedgedelqty = INT_MAX;
  (*pReg).maxff = 20;
  (*pReg).seltype = 0;
  (*pReg).sel = 0;
  (*pReg).rsel = 0;
  (*pReg).dboundseltype = 0;
  (*pReg).dboundsel = 0;
  (*pReg).dboundrsel = 0;
  (*pReg).dbound = NULL;

  return;
}

void
net_reg_free (struct REG Reg)
{
  ut_free_1d_char (Reg.regstring);
  ut_free_1d_int (Reg.reg);
  ut_free_1d_char (Reg.dbound);

  return;
}
