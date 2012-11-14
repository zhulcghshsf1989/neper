/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_mm.h"

void
TreatArg_mm (int fargc, char **fargv, int argc, char **argv,
	     struct IN *pIn)
{
  SetDefaultOptions_mm (pIn);

  /* Reading options. */
  SetOptions_mm (pIn, fargc, fargv);
  SetOptions_mm (pIn, argc, argv);

  if ((*pIn).input == NULL)
    ut_arg_badarg ();
  else if (! strcmp ((*pIn).input, "tess"))
  {
    if (ut_file_exist ((*pIn).ingeo) == 0)
      ut_file_open ((*pIn).ingeo, "r");
  }

  /* Writing file names */
  if ((*pIn).body == NULL)
  {
    (*pIn).body = ut_alloc_1d_char (1000);

    if ((*pIn).ingeo != NULL)
      ut_string_body ((*pIn).ingeo, (*pIn).body);
    else if ((*pIn).loadmesh != NULL)
      ut_string_body ((*pIn).loadmesh, (*pIn).body);
  }

  (*pIn).geo = ut_string_addextension ((*pIn).body, ".tess");
  (*pIn).gmshgeo = ut_string_addextension ((*pIn).body, ".geo");
  (*pIn).msh = ut_string_addextension ((*pIn).body, ".msh");
  (*pIn).geof = ut_string_addextension ((*pIn).body, ".geof");
  (*pIn).abq = ut_string_addextension ((*pIn).body, ".inp");

  return;
}
