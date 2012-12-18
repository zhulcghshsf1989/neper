/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../../../../structIn_t.h"

#include"ut.h"
#include"neut_t.h"
// #include"../../../../Meshing1D/Meshing1D0.h"
// #include"../../../../Meshing2D/Meshing2D0.h"

#include"FaceFF0.h"

#define verbosity 0

extern double FaceFFMesh (struct TESS *, struct TESSPARA, struct IN, int);
extern void FaceFFMeshTess (struct TESS *, struct TESSPARA, int *);
extern void FaceFFMeshGmsh (void);
extern double FaceFFMeshCalcFF (struct TESS *, int, int *, int);
extern double FaceTessFF (struct TESS *, int);
extern double FFBaryFace (struct TESS *, int);
/*
extern double ThisVerFaceFF2 (struct TESS *, struct TESSPARA, struct IN, int,
			      int);
*/
