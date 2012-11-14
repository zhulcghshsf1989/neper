/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEFM_INIT
#define NEFM_INIT

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"structAll.h"
#include"AllUtils.h"
#include"ut.h"
#include"neut.h"
#include"../structIn_fm.h"
#include"../ReconMesh/ReconMesh0.h"

#include"nefm_init0.h"

extern void rcl2cl_skin (double, int, struct GEOPARA *);
extern void cl2sel (struct GEOPARA*);

#endif /* NEFM_INIT */
