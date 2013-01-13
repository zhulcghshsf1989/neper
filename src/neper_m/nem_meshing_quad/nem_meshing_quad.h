/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"../structIn_m.h"
#include"ut.h"
#include"neut_m.h"
#include"../nem_reconstruct/nem_reconstruct0.h"
#include"./nem_meshing/nem_meshing_3D/nem_meshing_3D0.h"

#include"nem_meshing_quad0.h"

extern void nem_meshing_quadFTess (struct TESS Tess, struct NODES, struct MESH* pMesh);
extern void nem_meshing_quad_meshpoly (char*, struct VOX, struct MESH*, struct NODES*, struct NSET*);
extern void nem_meshing_quad_cleaning (struct MESH* pMesh, struct NODES* pNodes, int*** pFoDNodes);
extern void nem_meshing_quad_singnodedup (struct MESH* pMesh, struct NODES* pNodes, int*** pFoDNodes);