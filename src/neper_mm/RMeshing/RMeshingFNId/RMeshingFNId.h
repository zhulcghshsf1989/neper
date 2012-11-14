/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"structAll.h"
#include"AllUtils.h"
#include"../../../Utils/GslUtils/GslUtils0.h"
#include"ut.h"
#include"../../structIn_mm.h"
#include"../../../neper_t/IntePara/IntePara0.h"
#include"../../../neper_t/DomainComp/CubeDomain/CubeDomain0.h"
#include"../../../neper_t/PolyComp/PolyComp0.h"
#include"../../../neper_t/Tess/Tess0.h"
#include"../../../neper_t/Miscellaneous/Miscellaneous0.h"
#include"../../../neper_t/GermDistrib/GermDistrib0.h"

#include"RMeshingFNId0.h"
#include"../RMeshing0.h"

extern void NIdSearchEltPoly (int*, struct GERMSET*, struct MESH*);
