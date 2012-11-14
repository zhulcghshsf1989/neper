/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut.h"
#include"../../ArrayUtils/ArrayUtils0.h"

#include"T2P0.h"

extern void T2PVerQty (struct TESS, int, struct POLY *);
extern void T2PFaceQty (struct TESS, int, struct POLY *);

extern void T2PVerFace (struct TESS, int, struct POLY *, int *);
extern void T2PVerCoo (struct TESS, int, struct POLY *, int *);

extern void T2PFaceProp (struct TESS, int, struct POLY *, int *);
extern void T2PFacePoly (struct TESS, int, struct POLY *);
extern void T2PFaceEq (struct TESS, int, struct POLY *);
extern void T2PFaceVerQty (struct TESS, int, struct POLY *);
extern void T2PFaceVerNb (struct TESS, int, struct POLY *, int *);
