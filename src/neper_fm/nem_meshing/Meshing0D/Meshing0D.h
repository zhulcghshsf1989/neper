/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

#include"../../structIn_fm.h"
#include"neut.h"
#include"ut.h"

#include"Meshing0D0.h"

extern void VerMeshing (struct GEO, int, struct GEOPARA,
                        struct NODES*, struct MESH*);
