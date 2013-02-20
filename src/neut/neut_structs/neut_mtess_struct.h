/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMTESS_H
#define STRUCTMTESS_H

struct MTESS
{
  int LevelQty;          // number of levels (1 for a regular tessellation) 
  int* LevelTessQty;     // number of tesses at each level (indexed 1)
  int** LevelTess;       // Ids of the tesses at each level (indexed 1 1)

  int TessQty;		 // Grand nb of tessellations
  int** TessDom;         // Domain info of each tessellation
};
typedef struct MTESS MTESS;

#endif /* STRUCTMTESS_H */
