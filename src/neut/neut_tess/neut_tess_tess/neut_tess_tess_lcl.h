/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"neut.h"

#include"../neut_tess_tess.h"

extern void neut_tess_tessGen (struct TESS, struct TESS *);
extern void neut_tess_tessVer (struct TESS, struct TESS *);
extern void neut_tess_tessVerQty (struct TESS, struct TESS *);
extern void neut_tess_tessVerCoo (struct TESS, struct TESS *);
extern void neut_tess_tessVerEdge (struct TESS, struct TESS *);
extern void neut_tess_tessVerBound (struct TESS, struct TESS *);
extern void neut_tess_tessVerState (struct TESS, struct TESS *);
extern void neut_tess_tessEdge (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeQty (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeVerNb (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeFace (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeLength (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeState (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeDel (struct TESS, struct TESS *);
extern void neut_tess_tessEdgeBound (struct TESS, struct TESS *);
extern void neut_tess_tessFace (struct TESS, struct TESS *);
extern void neut_tess_tessFaceQty (struct TESS, struct TESS *);
extern void neut_tess_tessFacePoly (struct TESS, struct TESS *);
extern void neut_tess_tessFaceVer (struct TESS, struct TESS *);
extern void neut_tess_tessFaceEdge (struct TESS, struct TESS *);
extern void neut_tess_tessFaceFF (struct TESS, struct TESS *);
extern void neut_tess_tessFaceEq (struct TESS, struct TESS *);
extern void neut_tess_tessFaceBound (struct TESS, struct TESS *);
extern void neut_tess_tessFaceState (struct TESS, struct TESS *);
extern void neut_tess_tessFacePt (struct TESS, struct TESS *);
extern void neut_tess_tessPoly (struct TESS, struct TESS *);
extern void neut_tess_tessPolyQty (struct TESS, struct TESS *);
extern void neut_tess_tessPolyFace (struct TESS, struct TESS *);
extern void neut_tess_tessPolyCenter (struct TESS, struct TESS *);
extern void neut_tess_tessPolyTrue (struct TESS, struct TESS *);
extern void neut_tess_tessPolyBody (struct TESS, struct TESS *);
extern void neut_tess_tessDomain (struct TESS, struct TESS *);
extern void neut_tess_tessDomainData (struct TESS, struct TESS *);
