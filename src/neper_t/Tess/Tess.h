/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"structAll.h"
#include"AllUtils.h"
#include"ut.h"

#include"Tess0.h"

extern void IniTxNb (struct TESS, struct POLY *, int **, int **);
extern void IniTFNb (struct TESS, struct POLY *, int **);
extern void IniTVNb (struct TESS, struct POLY *, int **);
extern void TessAlloc (struct TESS *);
extern void IniTessFaceOr (struct TESS *, struct POLY *, int **);
extern void IniTessFaceVerQty (struct TESS *, struct POLY *, int **);
extern void IniTessFaceEq (struct TESS *, struct POLY *, int **);
extern void IniTessFacePoly (struct TESS *, struct POLY *, int **);
extern void IniTessFaceBound (struct TESS *, struct POLY *, int **, int **);
extern void IniTessVer (struct TESS *, struct POLY *, int **);
extern void IniTessVerGerm (struct TESS *, struct POLY *, int **);
extern void IniTessVerCoo (struct TESS *, struct POLY *, int **);
extern void IniTessPoly (struct TESS *, struct POLY *, int **, int **);
extern void IniTessPolyCenNXQty (struct GERMSET, struct TESS *,
				 struct POLY *);
extern void IniTessPolyXNb (struct TESS *, struct POLY *, int **, int **);
extern int IniPolyFaceOri (struct TESS *, int, int);
extern void IniTessFaceEdgeOri (struct TESS *);
extern int TreatVer (struct POLY *, int, int);
extern int AddVer (int **, int, int, int);
extern int PrevPoly (struct POLY *, int, int);
extern int FindVer (struct POLY *, int, int, int, int **);
extern int PrevVer (struct POLY *, int, int, int);
extern void ParFacePoly (struct POLY *, int, int, int, int *);
extern int SearchInPrevP (struct POLY *, int, int *);
extern void SearchVer (struct POLY *, int, int, int **);
extern void UpdTVNb (int **, int, int, int, int);
extern void IniTessEdge (struct TESS *);
extern void IniTessVernEdge (struct TESS *);
extern int CommonFaceTess (struct TESS, int, int, int *);
extern int CommonGerms (struct TESS, int, int, int *);
extern int BissFace (struct TESS, int, int);
extern int TreatFaceVer (struct TESS *, int, int, int);
extern void TessPolyDescription (int, struct TESS);

extern void TessGenDet (struct GERMSET, struct POLY *, struct TESS *);
extern int TFaceQty (struct GERMSET, struct POLY *);
extern int TVerQty (struct GERMSET, struct POLY *);
extern int TEdgeQty (struct GERMSET, struct POLY *);
extern int TPolyQty (struct GERMSET);
extern void IniTessPolyEdge (struct TESS *);
extern void Error (int);

extern int AddEdge (struct TESS *, int, int, int, int, int);
extern void RecEdgeVerNb (struct TESS *, int, int, int);
extern void RecFaceEdgeNb (struct TESS *, int, int, int);
extern void SearchEdge (struct TESS *, int, int, int, int, int);
extern void TessDes (struct TESS *);
extern int IntMin (int, int);
extern int RecPolyEdges (struct TESS *, int);
extern int FaceEdges2PolyEdges (struct TESS *, int, int, int);
extern int PrevFace (struct POLY *, int, int);
