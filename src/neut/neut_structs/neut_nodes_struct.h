/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTNODES_H
#define STRUCTNODES_H

struct NODES
{
  int NodeQty;			// total nb of nodes
                                // arrays are indexed [1...NodeQty]

  double **NodeCoo;		// for each node: coordinates, indexed [O...2]

  double* NodeCl;               // for each node: characteristic length
};
typedef struct NODES NODES;

#endif
