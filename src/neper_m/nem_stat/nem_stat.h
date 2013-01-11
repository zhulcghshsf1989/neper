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
#include"../nem_writemesh/nem_writemesh0.h"

#include"nem_stat0.h"

extern void nem_stat_nodes (FILE*, char*, struct NODES, struct MESH, struct MESH,
                struct MESH, struct MESH);
extern void nem_stat_elts  (FILE*, char*, struct NODES, struct MESH,
                            struct TESS);
extern void nem_stat_elsets (FILE*, char*, struct NODES, struct MESH,
	         struct MESHPARA, struct TESS);
