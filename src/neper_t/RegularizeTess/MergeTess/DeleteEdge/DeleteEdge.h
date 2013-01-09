/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"
// #include"../../../Meshing0D/Meshing0D0.h"

#include"FaceFF/FaceFF0.h"

#include"DeleteEdge0.h"


extern void UpdateEdgeState (struct TESS *, int);
extern void SearchDelNNewVer (struct TESS *, int, int *, int *);
extern void UpdateEdgeVerNb (struct TESS *, int, int, int);
extern void UpdateEdgeLength (struct TESS *, int);
extern int UpdateFaceVerNEdge (struct TESS *, int, int, int, int);
extern void UpdateFaceState (struct TESS *, int, int);
extern void UpdateFaceVer (struct TESS *, int, int, int);
extern int UpdateFaceEdge (struct TESS *, int, int);
extern void DeleteEdgeFromFace (struct TESS *, int, int, int);
extern void DeleteVerFromFace (struct TESS *, int, int, int);
extern void ReplaceVerInFace (struct TESS *, int, int, int);
extern int DeleteFace (struct TESS *, int, int, int);
extern void DeleteFaceFromItsPoly (struct TESS *, int);
extern void SearchNewNOldEdges (struct TESS *, int, int, int *, int *);
extern void nefm_edgedel_edgedom_fromverdom (struct TESS *, int);
extern void nefm_tess_reg_ver_facedom (struct TESS *, int);
extern int UpdateVer (struct TESS *, int, int, int, int);
extern void UpdateVerState (struct TESS *, int, int, int);
extern void UpdateVerEdge (struct TESS *, int, int, int, int);
extern int UpdateVerBound (struct TESS *, int, int);
extern int  UpdateVerCoo (struct TESS *, int, int);
extern int UpdateVerCooMiniFF (struct TESS *, int, int);
