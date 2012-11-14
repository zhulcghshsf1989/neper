/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTDATA_H
#define STRUCTDATA_H

struct DATA
{
  int*     qty;

  char*    entity;
  char*    type;

  double** val;

  // size stuff
  double*  size;

  // colour stuff
  int**    col;
  char*    colourscheme;
  char*    min;
  char*    max;
  double   height;
  char*    ticks;
};
typedef struct DATA DATA;

#endif /* STRUCTDATA_H */
