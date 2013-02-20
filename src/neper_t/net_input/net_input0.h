/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern void net_input (struct IN*, struct REG*, int, char **, int, char **);
extern void net_in_set_zero (struct IN*);
extern void net_in_free (struct IN);
extern void net_reg_set_zero (struct REG* pReg);
extern void net_reg_free (struct REG Reg);
