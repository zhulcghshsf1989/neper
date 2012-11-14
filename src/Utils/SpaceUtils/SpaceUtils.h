/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<gsl/gsl_rng.h>
#include<gsl/gsl_linalg.h>

#include"../MatrixUtils/VectorUtils0.h"
#include"../MatrixUtils/MatrixUtils0.h"
#include"../ArrayUtils/ArrayUtils0.h"
#include"../VectorUtils/VectorUtils0.h"
#include"ut.h"

#include"SpaceUtils0.h"

#include"neper_config.h"

extern double TriangleArea (double *, double *, double *);
extern void PolyCenter (double **, int, double *);
/* extern void PolygonTriangle (struct POLYGON, int, int, double **); 
extern double PolyhedronVolume (struct POLY); */
extern void Barycenter (double **, int, double *);

extern void ProjOnLine (double *, double *, double *);
extern void ProjOnPlane (double *, double *, double *);
