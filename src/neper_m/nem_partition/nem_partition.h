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

#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"
// #include<scotch.h>

#include "nem_partition0.h"

#ifdef HAVE_LIBSCOTCH

extern void nem_partitionNodes (struct IN, SCOTCH_Mesh *, SCOTCH_Arch *,
			    struct NODES *, struct MESH *, struct MESH *,
			    struct MESH *, struct MESH *, struct PART *);

extern void nem_partitionElts (struct IN, struct MESH *, struct PART *);
extern void nem_partitionElts_Match (struct IN, struct MESH *, struct PART *);

extern int nem_partitionElts_Balancing (struct MESH*, struct PART*, double);
extern void nem_partitionElts_ByNumber (struct IN, struct MESH *, struct PART *);
extern int nem_partitionElts_Balancing_Q (struct MESH, struct PART *, double, int,
				      int **);
extern void nem_partitionElts_Balancing_Q_elt (struct MESH, struct PART, int *,
					   int *);

extern void nem_partitionStats (int, struct MESH, struct PART);

#endif
