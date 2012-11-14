/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"InputData_o.h"

void
InputData_o (struct ORI *pOri, struct IN *pIn,
	     int fargc, char **fargv, int argc, char **argv)
{
  TreatArg_o (fargc, fargv, argc, argv, pOri, pIn);

  return;
}
