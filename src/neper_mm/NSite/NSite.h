/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<gsl/gsl_rng.h>
#include"../structIn_mm.h"
#include"ut.h"
#include"neut.h"
#include"../../neper_t/Miscellaneous/Miscellaneous0.h"

#include"NSite0.h"

extern void CalcZoneA (struct IN *, struct GERMSET, int *, int *, double ***,
		       int *, gsl_rng *);
extern void CalcZoneASize (struct IN *, struct GERMSET, int *);
extern void CalcZoneAGerms (struct GERMSET, int, int *, double ***, int *,
			    gsl_rng *);
extern void CalcNSiteNeigh (struct GERMSET, struct GERMSET *, int, double **,
			    double);
extern void CalcZoneB (struct GERMSET, struct GERMSET, int, int *, int *,
		       double ***, int *, gsl_rng *);
extern void CreateNSitenNeighTess (struct GERMSET, struct GERMSET, int,
				   double **, int, double **, struct TESS *);

extern void RecNSiteGeom (struct IN, struct GEO, int *, double ***, int ***);
extern void RecNSiteArray (struct IN, struct GEO, int, int *, double ***,
			   int ***);

extern void SearchNodePoly (struct GEO, struct NODES, int **);
extern void SearchSiteType (struct IN, struct GEO, struct NODES, struct MESH,
			    int, int *, double ***, int ***, int);

extern void DistribPoints (double *MaxSize, double *MinSize, int Qty,
    double **Data, gsl_rng * r);
