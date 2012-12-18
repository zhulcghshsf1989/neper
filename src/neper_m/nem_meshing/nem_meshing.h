/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<unistd.h>

#include"../structIn_m.h"
#include"neut_m.h"
#include"ut.h"

#include "nem_meshing_prep/nem_meshing_prep0.h"
#include "nem_meshing_0D/nem_meshing_0D0.h"
#include "nem_meshing_1D/nem_meshing_1D0.h"
#include "nem_meshing_2D/nem_meshing_2D0.h"
#include "nem_meshing_3D/nem_meshing_3D0.h"

#include"nem_meshing0.h"

extern void
nem_mesh_pinching (struct TESS Tess, double** face_eq, struct MESH
    Mesh1D, struct NODES Nodes, struct MESH* pMesh2D);
extern int
nem_remesh_fixmesh2delset (struct TESS Tess, struct MESH Mesh1D,
                           int face, struct MESH* pMesh2D);
