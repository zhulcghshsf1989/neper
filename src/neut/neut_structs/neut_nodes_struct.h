/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTNODES_H
#define STRUCTNODES_H

struct NODES
{
  int NodeQty;			/* total nb of nodes */
  double **NodeCoo;		/* nodes coordinates (O to 2) */

  double* NodeCl;

  double** BBox;
};
typedef struct NODES NODES;

#endif
