/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>

#include"../../structIn_m.h"
#include"ut.h"
#include"orilib.h"
#include"neut_m.h"
#include"../../nem/nem.h"
// #include"../nem_writemesh/nem_writemesh0.h"

#include"nem_meshing_prep0.h"

extern int nem_mesh2d_face_nproj (struct GEO,
    struct NODES, struct MESH, struct MESH, int, double*);
