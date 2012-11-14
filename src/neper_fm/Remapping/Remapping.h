/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<float.h>

#include"structAll.h"
#include"AllUtils.h"
#include"../structIn_fm.h"
#include"ut.h"
#include"orilib.h"
#include"neut.h"
#include"../Meshing0D/Meshing0D0.h"
#include"../Meshing1D/Meshing1D0.h"
#include"../Meshing2D/Meshing2D0.h"
#include"../Meshing3D/Meshing3D0.h"
#include"../nefm/nefm.h"

#include"Remapping0.h"

extern void nefm_remapping_mesh3d_parelt (struct GEO, struct NODES, struct MESH, struct MESH, struct NODES, struct MESH, struct MESH, int*);
