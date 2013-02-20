/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<gsl/gsl_matrix.h>
#include<gsl/gsl_linalg.h>
#include<gsl/gsl_sort.h>
#include<gsl/gsl_sort_vector.h>

#include"neut_t.h"
#include"../../net_utils/net_utils0.h"

#include"net_polycomp0.h"


extern void PolyhedronCalculation (struct POLY Domain, struct GERMSET GermSet, int PolyId, struct POLY *pPoly);

extern void NeiSearching (struct GERMSET GermSet, int germ, int *nbs);

extern int CurrentPolyTest (struct POLYMOD, double*, double*);

extern void NewPolyhedron (struct GERMSET GermSet, int PolyId, int Nei, struct POLYMOD* pPolymod);
extern void NewPolyhedron_plane (int NeiId, int CenterSide, double* plane, struct
    POLYMOD * pPolymod);

extern void PlanePara (int center, int nei, struct GERMSET GermSet, double *plane);

extern void BadVerDet (double *plane, int CenterSide, struct POLYMOD Polymod, int **pBadVer);

extern void
VerUseUpdating (POLYMOD * pPolymod, int *BadVer);
extern void
FaceUseUpdating (POLYMOD * pPolymod, int *BadVer);

extern int CheckFace (int *BadVer, struct POLYMOD Polymod, int face);
extern void NewFace (int Nei, POLYMOD * pPolymod, double *plane);
extern void FnLFacesDet (POLYMOD Polymod, int *BadVer, int *FnLFaces);
extern int FaceModif (int pFace, int Face, POLYMOD * pPolymod, int
  *BadVer, int last);
extern int NewVer (POLYMOD * pPolymod, int first, int second, int
  third);
extern void NewFaceAddVer (POLYMOD * pPolymod);

extern void UpdFaceQty (struct POLYMOD *pPolymod);
extern void UpdFaceEq (struct POLYMOD *pPolymod, double *plane);
extern void UpdFaceUse (struct POLYMOD *pPolymod);
extern void UpdFacePoly (struct POLYMOD *pPolymod, int Nei);
extern void UpdVerQty (struct POLYMOD *pPolymod);
extern void UpdVerUse (struct POLYMOD *pPolymod);
extern void UpdVerFace (struct POLYMOD *pPolymod, int first, int second, int third);
extern void UpdVerCoo (struct POLYMOD *pPolymod, int first, int second, int third);
extern void UpdFaceVerQty (struct POLYMOD *pPolymod);
extern void UpdFaceVerNb (struct POLYMOD *pPolymod);
extern void SnNS (struct POLYMOD Polymod, int ns, int nf, int *Ver, int *BadVer);
extern int NeiVer (struct POLYMOD Polymod, int face, int sum, int near);
extern int CommonFaces (POLYMOD pPolymod, int first, int second, int *res);
extern int VerCoupleNFaces (struct POLYMOD *pPolymod, int *BadVer, int Face, int pFace,
		 int *couple1, int *couple2, int *faces1, int *faces2,
		 int *pNbBadVer);
extern void FaceModifGenPara (struct POLYMOD *pPolymod, int Face, int
    NbBadVer);
extern void FaceModif1stNewVer (struct POLYMOD *pPolymod, int Face);
extern int FaceModif2ndNewVer (struct POLYMOD *pPolymod, int Face, int bel2, int *faces1,
		    int *faces2);
extern void FaceModifAddBegVer (struct POLYMOD *pPolymod, int Face, int last, int
    bel2);

extern int RotFaceVerNb (struct POLYMOD *pPolymod, int *BadVer, int Face);
