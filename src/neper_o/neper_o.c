/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neper_o.h"

int
neper_o (int fargc, char **fargv, int argc, char **argv)
{
/* *********************************************************
 * STRUCTURE AND VARIABLE DEFINITIONS  */
  struct ORI Ori;
  struct IN In;

  ut_print_moduleheader ("-O", fargc, fargv, argc, argv);

/* *********************************************************
 * Scanning the input data  */
  InputData_o (&Ori, &In, fargc, fargv, argc, argv);

/* *********************************************************
    Calculating angles  */
  if (In.input == 0 || In.input == 1)
    CalcOri (In, &Ori);
  else if (In.input > 1 && In.input <= 8)
    ReadOri (In, &Ori);

/* *********************************************************
 * RESULTS WRITING  */
  Res_o (In, Ori);

  if (In.colourspec != NULL)
    ColourOri (In, Ori);

  return 0;
}
