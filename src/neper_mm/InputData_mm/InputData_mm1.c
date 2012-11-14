/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "InputData_mm.h"

void
InputData_mm (struct IN *pIn, struct GERMSET* pGermSet,
	      int fargc, char **fargv, int argc, char **argv)
{
  TreatArg_mm (fargc, fargv, argc, argv, pIn, pGermSet);

  return;
}
