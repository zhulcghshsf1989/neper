/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTIN_H
#define STRUCTIN_H

struct IN
{
  int verbosity;
  char *gmsh;

  /* FILE NAMES */
  char* loadmesh;

  /* FFFS */
  int mloop;

  /* DEBUGGING STUFF */
  int maxedgedelqty;			/* this is for debugging */

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
  char *geo;
  char *ply;
  char *gmshgeo;
  char *ingeo;
  int printff;	         	
  char *ff;

  int printstattess;
  int printstatmesh;

  char *stt3;
  char *stt2;
  char *stt2a;
  char *stt0;
  char *stt1;
  char *stn;
  char *stm1;
  char *stm2;
  char *stm3;
  char *stm4;
  char *stm5;
  
  char *nstat;
  int printnstat;		/* if =1: printing node statistics  */

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
  char *eangle;
  int printeangle;		/* if =1: printing element angles      */
  char *fod;
  int printfod;
  char *oin;
  
  char *mast;
  char *asy;

  char* nodecoo;
};
typedef struct IN IN;

#endif /* STRUCTIN_H */
