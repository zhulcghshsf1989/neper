/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<unistd.h>

#include"../structIn_fm.h"
#include"ut.h"
#include"orilib.h"
#include"neut.h"
#include"../Meshing0D/Meshing0D0.h"
#include"../Meshing1D/Meshing1D0.h"
#include"../Meshing2D/Meshing2D0.h"
#include"../Meshing3D/Meshing3D0.h"
#include"../nefm/nefm.h"
#include"../WriteMesh/WriteMesh0.h"

#include"Premeshing0.h"

extern int nefm_mesh2d_face_nproj (struct GEO,
    struct NODES*, struct MESH*, struct MESH*,
    int, double*);

extern void nefm_geo_face_sim (struct GEO, int*, double***, double**);
