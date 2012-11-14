/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"structAll.h"
#include"../structIn_fm.h"
#include"AllUtils.h"
#include"ut.h"
#include"neut.h"
#include<gsl/gsl_sort_int.h>
#include"../WriteMesh/WriteMesh0.h"

#include"MStat0.h"

extern void PrintEStat (struct NODES, struct MESH, struct MESH, struct MESH, struct MESH, struct IN, struct GEOPARA, struct GEO*);
extern void CalcVol (struct NODES, struct MESH, double **);
extern void CalcRho (struct NODES, struct MESH, double **);
extern void CalcAngle (struct NODES, struct MESH, double ***);
extern void CalcRRatio (struct NODES, struct MESH, double**);
extern void CalcMeanLength (struct NODES, struct MESH, double**);
