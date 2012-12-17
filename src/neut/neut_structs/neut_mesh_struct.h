/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMESH_H
#define STRUCTMESH_H

struct MESH
{
  char* EltType;
  int Dimension;		/* Dimension of the mesh (0,1,2 or 3). */
  int EltOrder;			/* elt order */

  int EltQty;
  int **EltNodes;		/* EltNodes[i][0...]: nodes of elt i */

  int ElsetQty;			/* nb of elsets */
  int **Elsets;			/* Elsets[i][0,1...] elset qty of elts */
                                /* and their nbs */

  int* EltElset;

  int NodeQty;
  int** NodeElts;

  double** EltCoo;              // Used in -MM.
  int* msize;                   // Used in -MM.
};
typedef struct MESH MESH;

#endif /* STRUCTMESH_H */
