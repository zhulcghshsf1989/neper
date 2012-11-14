/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"gsl/gsl_sort_int.h"

#include"structAll.h"
#include"../../../structIn_fm.h"
#include"AllUtils.h"
#include"ut.h"
#include"neut.h"
#include"../../../Meshing0D/Meshing0D0.h"

#include"FaceFF/FaceFF0.h"

#include"DeleteEdge0.h"


extern void UpdateEdgeState (struct GEO *, int);
extern void SearchDelNNewVer (struct GEO *, int, int *, int *);
extern void UpdateEdgeVerNb (struct GEO *, int, int, int);
extern void UpdateEdgeLength (struct GEO *, int);
extern int UpdateFaceVerNEdge (struct GEO *, int, int, int, int);
extern void UpdateFaceState (struct GEO *, int, int);
extern void UpdateFaceVer (struct GEO *, int, int, int);
extern int UpdateFaceEdge (struct GEO *, int, int);
extern void DeleteEdgeFromFace (struct GEO *, int, int, int);
extern void DeleteVerFromFace (struct GEO *, int, int, int);
extern void ReplaceVerInFace (struct GEO *, int, int, int);
extern int DeleteFace (struct GEO *, int, int, int);
extern void DeleteFaceFromItsPoly (struct GEO *, int);
extern void SearchNewNOldEdges (struct GEO *, int, int, int *, int *);
extern void nefm_edgedel_edgedom_fromverdom (struct GEO *, int);
extern void nefm_geo_reg_ver_facedom (struct GEO *, int);
extern int UpdateVer (struct GEO *, int, int, int, int);
extern void UpdateVerState (struct GEO *, int, int, int);
extern void UpdateVerEdge (struct GEO *, int, int, int, int);
extern int UpdateVerBound (struct GEO *, int, int);
extern int  UpdateVerCoo (struct GEO *, int, int);
extern int UpdateVerCooMiniFF (struct GEO *, int, int);
