/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<gsl/gsl_linalg.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#include"structAll.h"
#include"../structIn_fm.h"
#include"AllUtils.h"
#include"ut.h"
#include"orilib.h"

#include"Meshing1D0.h"

extern void EdgeMeshing (struct GEO, int, double, double, struct
    NODES, struct NODES*, struct MESH*);
extern double NextCoo (double, double, double);
