/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTINTEPARA_H
#define STRUCTINTEPARA_H

struct INTEPARA
{
  /* MaxToSort: prior to the tessellation process, maximum number of
   * neighbours to find and sort for every germ */
  int MaxToSort;
};
typedef struct INTEPARA INTEPARA;

#endif /* STRUCTINTEPARA_H */
