/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTVOX_H
#define STRUCTVOX_H

struct VOX
{
  // GENERAL INFORMATION -----------------------------------------------
  char* version;
  char* format;

  int* size;                  // number of voxels in x, y, z
  double* vsize;              // size of voxels in x, y, z

  int*** VoxPoly;             // Poly of a voxel, indexed [0...size[0]-1], etc.

  int PolyQty;                // Number of grains
};
typedef struct VOX VOX;

#endif /* STRUCTVOX_H */
