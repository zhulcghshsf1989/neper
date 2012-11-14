/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

struct IN
{
  int input;   // -1: none, 0: (n, id), 1: tess, 2: regular

  char* domain;
  double* domainparms;
  char* domainparms2;
  double *F;
  int verbosity;

  char *format;
  char *body;
  char *load;
  char *geo;
  char *gmshgeo;
  char *ply;
  char *oin;
  char *mast;
  char *asy;
  char *stt3;
  char *stt0;
  char *neigh;
  char *debug;

  int stattess;
  int sorttess_qty;
  char** sorttess;

  int printneighbour;
  int printpointpoly;		/* 0: no, 1: input=points, 2: input=tess file */
  char *point;			/* point or tess file name */
  char *polyid;			/* polyid file name */

  char* libpath;

  int centroid;
  int centroiditermax;
  double centroidfact;
  double centroidconv;
};
typedef struct IN IN;
