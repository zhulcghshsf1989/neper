/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTGERMSET_H
#define STRUCTGERMSET_H

struct GERMSET
{
  // GENERAL INFORMATION -----------------------------------------------

  double* Size;       // size of the germ set (= bounding box of the
                      // domain) 

  char* morpho;       // morphology (poisson|cube|dodeca|tocta|custom)

  long Random;        // Number used for the germ generation process.

  int Id;             // identifier of the distribution

  // GERM DISTRIBUTION -------------------------------------------------

  int N;              // Number of germs.
  int N1d;            // Variant of N. Number of germs in 1D
                      // (for cube and tocta tessellations)
  int NDensity;       // Variant of N. Nominal density (for subdomain)

  // For germ i (i=1...N):
  double** GermCoo;  // GermCoo[i][j] (j=0...2) are the 3 coordinates.

  // DOMAIN NEIGHBOURHOOD GERM DISTRIBUTION ----------------------------

  int nN;	       // Number of germs in the domain neighbourhood
  double *nSize;       // Size of the domain neighbourhood 
  double nVol;	       // Volume
  double** nGermCoo;   // nGermCoo[i][j] (j=0...2) are the 3 coordinates.

  // this variable is specific to periodic tessellations
  // For germ i (i=1...nN):
  int *nMaster;        // Master of the germ (it is inside the domain)

  // For germ i (i=1...nN):
  // this variable is specific to sudomain-type tessellations
  int *nGermRandId;    // Germs rng output id
};
typedef struct GERMSET GERMSET;

#endif /* STRUCTGERMSET_H */
