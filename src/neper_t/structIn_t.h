/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_T_H
#define STRUCTIN_T_H

struct IN
{
  int n;
  int id;
  char* morpho;
  char* ttype;

  double randomize;
  int    randomize2;
  char*  randomizedir;
  
  char* mode;    // running mode: tess or vox
  char* input;   // NULL (=none), n, tess, vox

  char* domain;
  double* domainparms;
  char* domainparms2;
  double *scale;

  int voxsizetype;
  int voxsize;
  int* voxsize3;

  int checktess;

  char *format;
  char *voxformat;
  char *body;
  char *load;
  char *tess;
  char *vox;
  char *geo;
  char *ply;
  char *stv;
  char *ste;
  char *stf;
  char *stp;
  char *dec;
  char *debug;

  char* printstattess;
  int sorttess_qty;
  char** sorttess;

  int printpointpoly;		/* 0: no, 1: input=points, 2: input=tesl file */
  char *point;			/* point or tesl file name */
  char *polyid;			/* polyid file name */

  int centroid;
  int centroiditermax;
  double centroidfact;
  double centroidconv;
};
typedef struct IN IN;

#endif /* STRUCTIN_T_H */
