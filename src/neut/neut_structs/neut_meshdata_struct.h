/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMESHDATA_H
#define STRUCTMESHDATA_H

struct MESHDATA
{
  int elset0dqty;
  int elset1dqty;
  int elset2dqty;
  int elset3dqty;

  int elt0dqty;
  int elt1dqty;
  int elt2dqty;
  int elt3dqty;
  int nodeqty;

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

  double*** coodata;
  char**    coodatatype;
  double*** coo;
  double*   coofact;
};
typedef struct MESHDATA MESHDATA;

#endif /* STRUCTMESHDATA_H */
