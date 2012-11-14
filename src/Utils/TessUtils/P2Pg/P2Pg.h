/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>

#include"neut.h"
#include"../../Structs/structPolygon.h"
#include"../../ArrayUtils/ArrayUtils0.h"

#include"P2Pg0.h"

extern void P2PgVerQty (struct POLY, int, struct POLYGON *);
extern void P2PgEq (struct POLY, int, struct POLYGON *);
extern void P2PgVerCoo (struct POLY, int, struct POLYGON *);
extern void P2PgCenterCoo (struct POLYGON *);
