/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void InputData_fm (struct IN*, struct GEOPARA *, int, char **,
    int, char **);
extern void nefm_in_set_zero (struct IN*);
extern void nefm_in_free (struct IN);
extern void nefm_geopara_set_zero (struct GEOPARA*);
extern void nefm_geopara_free (struct GEOPARA);
