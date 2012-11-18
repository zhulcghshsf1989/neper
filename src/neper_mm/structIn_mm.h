/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

struct IN
{
  /* FILE NAMES */
  int mesh;	
  char* loadmesh;
  char* nodecoo;
  char* domain;
  double* domainparms;
  char* domainparms2;
  double* F;

  int msizetype;
  int msize;
  int* msize3;
  int morder;
  int input;   // -1: none, 0: (n, id), 1: tess, 2: regular

  char *ingeo;
  char *body;
  char *nset;
  char* faset;			/* list the nsets: none (0), faces (1) */

  char* outdim;
  int nsite;
  double nsitesize;
  char *nsitefile;
  char* format;
  char *centrecoo;

  char* geo;
  char* gmshgeo;
  char* msh;
  char* geof;
  char* rset;
  char* abq;
  char* oin;
  char* nper;

  int verbosity;
};
typedef struct IN IN;
