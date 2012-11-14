/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"../structIn_fm.h"
#include"ut.h"
#include"neut.h"
#include<gsl/gsl_sort_int.h>
#include"../WriteMesh/WriteMesh0.h"

#include"nem_stat0.h"

extern void nem_stat_nodes (FILE*, char*, struct NODES, struct MESH, struct MESH,
                struct MESH, struct MESH);
extern void nem_stat_elts  (FILE*, char*, struct NODES, struct MESH,
                            struct GEO*);
extern void nem_stat_elsets (FILE*, char*, struct NODES, struct MESH,
	         struct GEOPARA, struct GEO*);
