/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTGERMSET_H
#define STRUCTGERMSET_H

struct GERMSET
{
  char* morpho;
  int N;
  int NDensity;
  int N1d;
  char* ttype;
  int Id;
  long Random;     

  double** GermsCoo;

  double randomize;
  int    randomize2;
  char*  randomizedir;

  // Domain neighbourhood info
  double* Size;
  int nN;	       // real nb of germs
  double *nSize;       // Size (x,y,z bounds)
  double nVol;	       // Real volume
  double **nGermsCoo;  // Germs coo (these are outside the domain)
  int *nGermRandId;    // Germs rng output id
  int *GermToGerm;     // This is useful for ttype=1
};
typedef struct GERMSET GERMSET;

#endif /* STRUCTGERMSET_H */
