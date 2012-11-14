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

  int* size;
  double* vsize;

  int*** VoxPoly;

  int PolyQty;
};
typedef struct VOX VOX;

#endif /* STRUCTVOX_H */
