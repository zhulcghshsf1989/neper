/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_H
#define STRUCTIN_H

struct IN
{
  char *gmsh;

  /* FILE NAMES */
  char* loadmesh;

  /* MESH */
  int morder;
  int meshdim;

  char* meshpoly;
  char* meshface;

  char* outdim;

  char *format;			/* geo, msh, geof and/or fev */
  char* mesh2dalgo;		/* 2d algo: 1: iso, 2: aniso, 3: triangle */
  char* mesh3dalgo;		/* 3d algo: 1: iso, 2: aniso, 3: triangle */
  char* mesh3doptiexpr;
  char* mesh3doptidisexpr;
  double mesh2dmaxtime;
  double mesh2drmaxtime;
  int    mesh2diter;
  double mesh3dmaxtime;
  double mesh3drmaxtime;
  int    mesh3diter;
  int    mesh3dreport;
  double mesh3dclconv;

  char *nset;		/* list of nsets (sep by ',') if nset = 0 */
  char* faset;          /* surfs to print */

  /* operation to do */
  int mesh;	
  int remesh;	
  int remap;

  char* partmethod;
  int partqty;
  int partsets;
  double partbalancing;
  int partrenumber;
  int partmode;
  char* partarchfile;
  char* npart;
  char* epart;
  char*   remap2;
  char*** remapspec;

  char *body;
  char *ingeo;

  char *stn;
  char *ste;
  char *stelset;
  
  char *remesh2;
  char *remeshtess;
  char *transporttess;
  char *rem1;
  char *msh;
  char *abq;
  char *geof;
  char *stenode;
  char *steele;
  char *scm;
  char *scg;
  char *fev1;
  char *fev2;
  char *fev3;
  char *fev4;
  char *fev5;
  
  char* nodecoo;
};
typedef struct IN IN;

#endif /* STRUCTIN_H */
