/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

struct ORI
{
  /* input = N,Id */
  int N;
  int id;

  /* input = oin file */
  int *rngid;

  int Random;

  /* res */
  /*char* ori; */
  double **q;
};
typedef struct ORI ORI;
