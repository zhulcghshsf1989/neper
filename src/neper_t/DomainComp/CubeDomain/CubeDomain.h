/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"

#include"CubeDomain0.h"

extern void CubeVer (double *, struct POLY *);
extern void IniVerQty (struct POLY *);
extern void IniVerFace (struct POLY *);
extern void IniVerCoo (double *, struct POLY *);
extern void IniVerUse (struct POLY *);

extern void CubeFaces (double *, struct POLY *);
extern void IniFaceQty (struct POLY *);
extern void IniFaceEq (double *, struct POLY *);
extern void IniFacePoly (struct POLY *);
extern void IniFaceVerQty (struct POLY *);
extern void IniFaceVerNb (struct POLY *);
extern void IniFaceUse (struct POLY *);
