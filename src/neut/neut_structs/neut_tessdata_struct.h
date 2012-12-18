/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTTESSDATA_H
#define STRUCTTESSDATA_H

struct TESSDATA
{
  int       verqty;
  int       edgeqty;
  int       faceqty;
  int       polyqty;

  double*** coldata;
  char**    coldatatype;
  char**    colscheme;
  int***    col;
  double*** raddata;
  char**    raddatatype;
  double**  rad;
  char**    scalemin;
  char**    scalemax;
  char**    scaleticks;
};
typedef struct TESSDATA TESSDATA;

#endif /* STRUCTTESSDATA_H */
