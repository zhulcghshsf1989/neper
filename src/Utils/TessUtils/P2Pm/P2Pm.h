/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut.h"

#include"P2Pm0.h"

extern void P2PmVerQty (struct POLY, struct POLYMOD *);
extern void P2PmFaceQty (struct POLY, struct POLYMOD *);
extern void P2PmVerProp (struct POLY, struct POLYMOD *);
extern void P2PmFaceProp (struct POLY, struct POLYMOD *);
extern void P2PmVerFace (struct POLY, struct POLYMOD *);
extern void P2PmVerCoo (struct POLY, struct POLYMOD *);
extern void P2PmVerUse (struct POLYMOD *);
extern void P2PmFacePoly (struct POLY, struct POLYMOD *);
extern void P2PmFaceEq (struct POLY, struct POLYMOD *);
extern void P2PmFaceVerQty (struct POLY, struct POLYMOD *);
extern void P2PmFaceVerNb (struct POLY, struct POLYMOD *);
extern void P2PmFaceUse (struct POLYMOD *);
