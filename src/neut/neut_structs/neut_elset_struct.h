/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTELSET_H
#define STRUCTELSET_H

struct ELSET
{
  int Qty;			/* Qty of elsets */
  char **Names;			/* their names */
  int **Elset;			/* elset elts [0]=elt qty */
  char *Body;			/* body name, ie 'poly' */
  int *Id;			/* Id of the elsets */
};
typedef struct ELSET ELSET;

#endif /* STRUCTELSET_H */
