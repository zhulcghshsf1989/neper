/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTPRINT_H
#define STRUCTPRINT_H

struct PRINT
{
  // camera settings
  char**  cameracooexpr;
  double* cameracoo;

  char**  cameralookatexpr;
  double* cameralookat;

  double* camerasky;
  double  cameraangle;

  char*   cameraprojection;

  // image settings
  double* imagebackground;
  int imagewidth;
  int imageheight;
  int imageantialias;
  
  int showinit;
  int cameralookatinit;
  int showverinit;
  int showedgeinit;

  int showtess;
  int showtesslevel;
  int showmesh;
  char* showslice;

  int* shownode;
  int* showelt;
  int* showeltedge;
  int* showelt2d;
  int* showelt1d;
  int* showelt0d;
  int** showelt1dall;
  int showelt1dallqty;

  int* showver;
  int* showedge;
  int* showface;
  int showfaceinter;
  int* showpoly;
  
  int showshadow;
  
  // miscellaneous
  int datareduction;

  char* format;

  char* includepov;

  char* slice;
};
typedef struct PRINT PRINT;

#endif /* STRUCTPRINT_H */
