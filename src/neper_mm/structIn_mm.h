/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

struct IN
{
  /* FILE NAMES */
  int mesh;	
  char* loadmesh;
  char* domain;
  double* domainparms;
  char* domainparms2;

  int msizetype;
  int msize;
  int morder;
  int* msize3;
  char* input;

  char* ingeo;
  char* body;
  char* nset;
  char* faset;			/* list the nsets: none (0), faces (1) */

  char* outdim;
  char* meshpoly;
  int cleaning;
  int singnodedup;
  
  char* format;
  char* centrecoo;

  char* geo;
  char* gmshgeo;
  char* msh;
  char* geof;
  char* abq;
};
typedef struct IN IN;
