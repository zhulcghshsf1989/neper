/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include"ut.h"
#include"neut.h"
#include"orilib.h"

#include"../nevs_data_utils/nevs_data_utils0.h"
#include "nevs_meshdata0.h"

extern void nevs_meshdata_fscanf_set2entity (struct MESH,
		   struct MESH, char*, char*, struct MESHDATA*);
