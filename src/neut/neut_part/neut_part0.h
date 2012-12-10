/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_PART0_H
#define NEUT_PART0_H

#include"../neut_structs.h"

extern void neut_part_set_zero (struct PART*);
extern void neut_part_free (struct PART);
extern void neut_part_fprintf (char*, struct PART);

#endif /* NEUT_PART0_H */
