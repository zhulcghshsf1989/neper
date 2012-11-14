/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"../../structIn_fm.h"
#include"ut.h"
#include"neut.h"
#include "WriteMeshAbq0.h"

extern void neut_mesh_fprintf_abq_nsets (FILE*, struct NSET, struct NSET, struct NSET, char*);
extern void neut_mesh_fprintf_abq_nset (FILE*, char*, int, int*);
