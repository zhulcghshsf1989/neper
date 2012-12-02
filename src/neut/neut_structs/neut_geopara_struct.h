/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTGEOPARA_H
#define STRUCTGEOPARA_H

struct GEOPARA
{
  int cltype;
  double cl;	
  double rcl;
  double* cl3;
  double* rcl3;
  double clmin;
  double pcl;	
  double maxff;

  int msizetype;
  int msize;
  int* msize3;

  int seltype;
  double sel;
  double rsel;

  char* dbound; /* "true>2", "body>1", ... */
  double dboundcl;  /* counterpart of   cl for skin entities */
  double dboundrcl; /* counterpart of  rcl for skin entities */
  double dboundpcl; /* counterpart of  pcl for skin entities */
  int dboundseltype;
  double dboundsel; /* counterpart of  sel for skin entities */
  double dboundrsel;/* counterpart of rsel for skin entities */

  int verrcl_ver;
  double verrcl_rcl;
  double verrcl_cl;
  
  /*int      forcemergeqty;    force  merging, qty of edges */
  /*int*     forcemerge;       force  merging, ids of edges */
  /*int      forbidmergeqty;   forbid merging, qty of edges */
  /*int*     forbidmerge;      forbid merging, ids of edges */

}; typedef struct GEOPARA GEOPARA;

#endif /* STRUCTGEOPARA_H */
