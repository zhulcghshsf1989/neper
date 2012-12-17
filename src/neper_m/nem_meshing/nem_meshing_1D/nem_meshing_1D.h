/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<gsl/gsl_linalg.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>

#include"../../structIn_m.h"
#include"neut_m.h"
#include"ut.h"
#include"orilib.h"

#include"nem_meshing_1D0.h"

extern void EdgeMeshing (struct GEO, int, double, double, struct
    NODES, struct NODES*, struct MESH*);
extern double NextCoo (double, double, double);
extern void nem_mesh_1d_unitsegment (double cl1, double cl2, double clt, double pl,
                     int* pqty, double** pcoo, double** pcl);
