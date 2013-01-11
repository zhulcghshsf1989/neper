/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMESH_H
#define STRUCTMESH_H

struct MESH
{
  // mesh properties (all elements are of the same type / dimension / order)

  char* EltType;                // type of elements (tri or quad)
  int Dimension;		// Dimension (0, 1, 2 or 3)
  int EltOrder;			// Elt order (1 or 2)

  // information on the elements and element sets

  int EltQty;                   // total number of elements
                                // arrays are indexed [1...EltQty]

  int **EltNodes;		// for each elt: nodes, indexed [0...]

  int ElsetQty;			// total number of elsets
                                // arrays are indexed [1...ElsetQty]

  int **Elsets;			// for each elset:
                                // [0]: total number of elts
                                // [1...Elsets[][0]]: ids of the elts

  int* EltElset;                // for each elt: elset

  // information on the nodes

  int NodeQty;                  // total number of nodes

  int** NodeElts;               // for each node:
                                // [0]: total number of elts
                                // [1...NodeElts[][0]]: ids of the elts
};
typedef struct MESH MESH;

#endif /* STRUCTMESH_H */
