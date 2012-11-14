/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTNSET_H
#define STRUCTNSET_H

struct NSET
{
  int qty;	// quantity of nsets (recorded 1 ... qty)

  char** names; // names
  int* nodeqty; // number of nodes
  int** nodes;  // 0 indexed
};
typedef struct NSET NSET;

#endif /* STRUCTNSET_H */
