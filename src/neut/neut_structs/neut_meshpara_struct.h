/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTMESHPARA_H
#define STRUCTMESHPARA_H

struct MESHPARA
{
  int cltype;
  double cl;	
  double rcl;
  double* cl3;
  double* rcl3;
  double clmin;
  double pcl;	

  int msizetype;
  int msize;
  int* msize3;

  char* dbound; /* "true>2", "body>1", ... */
  double dboundcl;  /* counterpart of   cl for skin entities */
  double dboundrcl; /* counterpart of  rcl for skin entities */
  double dboundpcl; /* counterpart of  pcl for skin entities */

  int verrcl_ver;
  double verrcl_rcl;
  double verrcl_cl;

}; typedef struct MESHPARA MESHPARA;

#endif /* STRUCTMESHPARA_H */

