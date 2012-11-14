/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<limits.h>
#include<assert.h>
#define frand() ((double) rand() / (RAND_MAX+1.0))

#include"../structIn_fm.h"
#include"ut.h"
#include"neut.h"
// #include<scotch.h>

#include "Partition0.h"

#ifdef HAVE_LIBSCOTCH

extern void PartitionNodes (struct IN, SCOTCH_Mesh *, SCOTCH_Arch *,
			    struct NODES *, struct MESH *, struct MESH *,
			    struct MESH *, struct MESH *, struct NSET *,
			    struct NSET *, struct NSET *, struct PART *);

extern void PartitionElts (struct IN, struct MESH *, struct PART *);
extern void PartitionElts_Match (struct IN, struct MESH *, struct PART *);

extern int PartitionElts_Balancing (struct MESH*, struct PART*, double);
extern void PartitionElts_ByNumber (struct IN, struct MESH *, struct PART *);
extern int PartitionElts_Balancing_Q (struct MESH, struct PART *, double, int,
				      int **);
extern void PartitionElts_Balancing_Q_elt (struct MESH, struct PART, int *,
					   int *);

extern void PartitionStats (int, struct MESH, struct PART);

#endif
