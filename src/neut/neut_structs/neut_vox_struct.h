/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTVOX_H
#define STRUCTVOX_H

struct VOX
{
  // GENERAL INFORMATION -----------------------------------------------

  int* size;          // number of voxels in the 3 directions of space
  double* vsize;      // size of voxels in the 3 directions of space

  // DATA --------------------------------------------------------------

  int*** VoxPoly;     // VoxPoly[i][j][k]: poly of pixel at
                      // position i, j, k, with i in [1...size[0]],
                      // j in [1...size[1]] and k in [1...size[2]]

  int PolyQty;        // number of polyhedra
};
typedef struct VOX VOX;

#endif /* STRUCTVOX_H */
