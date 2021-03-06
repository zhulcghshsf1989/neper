/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef NEUT_NSET0_H
#define NEUT_NSET0_H

extern void neut_nset_expand (struct NSET, struct NSET, struct NSET, char*, char**);
extern void neut_nset_set_zero (struct NSET*);
extern void neut_nset_free (struct NSET*);

#endif /* NEUT_NSET0_H */
