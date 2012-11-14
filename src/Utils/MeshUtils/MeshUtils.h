/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

#include"ut.h"
#include"neut.h"
#include"../ArrayUtils/ArrayUtils0.h"
#include"../SpaceUtils/SpaceUtils0.h"
#include"../NumUtils/NumUtils0.h"
#include"../FileUtils/FileUtils0.h"
#include"../CharUtils/CharUtils0.h"
#include"structAll.h"

#include"MeshUtils0.h"
#include"neper_config.h"

extern void FoD2EoDNodes (int **, int ***);
extern void EoD2CoDNodes (int **, int ***);
extern void FoD2FoDBNodes (int **, int **, int ***);
extern void EoD2EoDBNodes (int **, int **, int ***);

extern void WriteNodeFree (int, int, double, int, FILE *);
