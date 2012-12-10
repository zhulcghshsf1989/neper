/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut.h"
#include "neut_mesh_fprintf_inp0.h"

extern void neut_mesh_fprintf_inp_nsets (FILE*, struct NSET, struct NSET, struct NSET, char*);
extern void neut_mesh_fprintf_inp_nset (FILE*, char*, int, int*);
