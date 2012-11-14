/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTSCALE_H
#define STRUCTSCALE_H

struct SCALE
{
  char*  colourscheme;
  char*  minexpr;
  char*  maxexpr;
  double min;
  double max;

  double height;
  char*  val;
};
typedef struct SCALE SCALE;

#endif /* STRUCTSCALE_H */
