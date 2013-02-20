/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTREG_H
#define STRUCTREG_H

struct REG
{
  char* regstring;
  int* reg;
  int mloop;
  int maxedgedelqty;
  double maxff;
  int seltype;
  double sel;
  double rsel;
  int dboundseltype;
  double dboundsel; /* counterpart of  sel for skin entities */
  double dboundrsel;/* counterpart of rsel for skin entities */
  char* dbound; /* "true>2", "body>1", ... */
}; typedef struct REG REG;

#endif /* STRUCTREG_H */
