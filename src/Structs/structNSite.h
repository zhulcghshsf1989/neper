/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

struct NSITE
{
  double t;			/* time at which the site has nucleated */
  double *Coo;			/* Coo[0...2]: position of the site */
  double *e;			/* Orientation */
  double priority;		/* random nb bw 0 and 1 for priority
				   during transformation */
};
typedef struct NSITE NSITE;
