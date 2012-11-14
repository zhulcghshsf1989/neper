/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"structAll.h"
#include"../../../../structIn_fm.h"

#include"AllUtils.h"
#include"ut.h"
#include"neut.h"
#include"../../../../Meshing1D/Meshing1D0.h"
#include"../../../../Meshing2D/Meshing2D0.h"

#include"FaceFF0.h"

#define verbosity 0

extern double FaceFFMesh (struct GEO *, struct GEOPARA, struct IN, int);
extern void FaceFFMeshGeo (struct GEO *, struct GEOPARA, int *);
extern void FaceFFMeshGmsh (void);
extern double FaceFFMeshCalcFF (struct GEO *, int, int *, int);
extern double FaceGeoFF (struct GEO *, int);
extern double FFBaryFace (struct GEO *, int);
/*
extern double ThisVerFaceFF2 (struct GEO *, struct GEOPARA, struct IN, int,
			      int);
*/
