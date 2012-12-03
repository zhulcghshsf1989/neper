/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"../structIn_fm.h"
#include"ut.h"
#include"neut.h"
#include"../ReconMesh/ReconMesh0.h"
#include"./nem_meshing/Meshing3D/Meshing3D0.h"

#include"RMeshing0.h"

extern void RMeshingFGeo (struct GEO Geo, struct MESH* pMesh);
extern void RMeshing_meshpoly (char*, struct VOX, struct MESH*, struct NODES*, int***);
extern void RMeshing_cleaning (struct MESH* pMesh, struct NODES* pNodes, int*** pFoDNodes);
extern void RMeshing_singnodedup (struct MESH* pMesh, struct NODES* pNodes, int*** pFoDNodes);
