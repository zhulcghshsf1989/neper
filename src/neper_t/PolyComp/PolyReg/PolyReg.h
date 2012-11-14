/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"structAll.h"
#include"AllUtils.h"
#include"ut.h"

#include"PolyReg0.h"

extern void Pm2PComp (struct POLYMOD, int *, int *, int *, int *);
extern void Pm2PVerQty (struct POLY *, int, int *);
extern void Pm2PFaceQty (struct POLY *, int, int *);
extern void Pm2PCompVerNVerInv (struct POLYMOD, int *, int *);
extern void Pm2PCompFaceNFaceInv (struct POLYMOD, int *, int *);
extern void Pm2PVerCoo (struct POLYMOD, struct POLY *, int *, int);
extern void Pm2PVerFace (struct POLYMOD, struct POLY *, int *, int *, int);
extern void Pm2PFacePoly (struct POLYMOD, struct POLY *, int *, int);
extern void Pm2PFaceEq (struct POLYMOD, struct POLY *, int *, int);
extern void Pm2PFaceVerQty (struct POLYMOD, struct POLY *, int *, int);
extern void Pm2PFaceVerNb (struct POLYMOD, struct POLY *, int *, int *, int);

extern void PolyDescription (int, struct POLY);
extern void PolymodDescription (int, struct POLYMOD);
