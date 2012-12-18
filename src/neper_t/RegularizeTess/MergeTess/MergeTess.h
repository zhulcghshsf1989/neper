/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<float.h>
#include<unistd.h>

#include"../../structIn_t.h"
#include"ut.h"
#include"neut_t.h"

#include"DeleteEdge/DeleteEdge0.h"
#include"MergeTess0.h"

extern int NextEdgeToDel (struct TESS, struct TESSPARA, double *);
extern void PrintDeletion (int, double, int, int, int, double, double);