/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"PolyCalc0.h"

#include"neut_t.h"
#include"ut.h"

extern void NewPolyhedron (int, int, struct GERMSET, struct POLYMOD *);
extern void PlanePara (int, int, struct GERMSET, double *);
extern void NewFace (int, struct POLYMOD *, double *);
extern void BadVerDet (int, double *, struct GERMSET, struct POLYMOD, int**);
extern void VerUseUpdating (struct POLYMOD *, int *);
extern void FaceUseUpdating (struct POLYMOD *, int *);
extern int CheckFace (int *, struct POLYMOD, int);

extern int FaceModif (int, int, struct POLYMOD *, int *, int);
extern int VerCoupleNFaces (struct POLYMOD *, int *, int, int, int *, int *,
			    int *, int *, int *);
extern void FaceModifGenPara (struct POLYMOD *, int, int);
extern void FaceModif1stNewVer (struct POLYMOD *, int);
extern int FaceModif2ndNewVer (struct POLYMOD *, int, int, int *, int *);
extern void FaceModifAddBegVer (struct POLYMOD *, int, int, int);
extern void FnLFacesDet (struct POLYMOD, int *, int *);
extern double MaxiDist (int, struct GERMSET, struct POLYMOD);
extern int CurrentPolyTest (double, double);

extern int NewVer (struct POLYMOD *, int, int, int);
extern void NewFaceAddVer (struct POLYMOD *);
extern int CommonFaces (struct POLYMOD, int, int, int *);

extern void UpdFaceQty (struct POLYMOD *);
extern void UpdFaceEq (struct POLYMOD *, double *);
extern void UpdFaceUse (struct POLYMOD *);
extern void UpdFacePoly (struct POLYMOD *, int);
extern void UpdVerQty (struct POLYMOD *);
extern void UpdVerUse (struct POLYMOD *);
extern void UpdVerFace (struct POLYMOD *, int, int, int);
extern void UpdVerCoo (struct POLYMOD *, int, int, int);
extern void UpdFaceVerQty (struct POLYMOD *);
extern void UpdFaceVerNb (struct POLYMOD *);
extern int RotFaceVerNb (struct POLYMOD *, int *, int);

extern void SnNS (struct POLYMOD, int, int, int *, int *);
extern int NeiVer (struct POLYMOD, int, int, int);

extern void PolyDescription (int, struct POLY);
extern void PolymodDescription (int, struct POLYMOD);
extern void Error (int);
extern void NeiSearching (struct GERMSET, int, int, int *, double *);
