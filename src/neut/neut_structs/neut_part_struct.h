/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef STRUCTPART_H
#define STRUCTPART_H

struct PART
{
  int qty;	/* quantity of partitions */

  int* node_parts; /* new node numbers */
  int* elt_parts;  /* new elt  numbers */

  int* nodeqty; /* quantities of nodes in the partitions (0-based) */
  int* eltqty;  /* quantities of elts  in the partitions (0-based) */
  int* targeteltqty; /* target quantities of elts in the partitions
			(0-based)*/
};
typedef struct PART PART;

#endif /* STRUCTPART_H */
