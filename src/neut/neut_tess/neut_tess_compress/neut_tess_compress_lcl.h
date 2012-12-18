/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"neut.h"
#include"../neut_tess_compress.h"

extern void neut_tess_compress_moveface (struct TESS*, int, int);
extern void neut_tess_compress_moveedge (struct TESS*, int, int);
extern void neut_tess_compress_movever  (struct TESS*, int, int);
