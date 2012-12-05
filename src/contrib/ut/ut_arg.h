/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_ARG_H
#define UT_ARG_H

extern int ut_arg_nextasint (char **, int *, char *, int, int);
extern int ut_arg_nextaslogical (char **, int *, char *);
extern double ut_arg_nextasreal (char **, int *, char *, double, double);
extern char *ut_arg_nextaschar (char **, int *, char *);
extern void ut_arg_badarg (void);
extern void ut_arg_error (char *, char*);
extern int ut_arg_listqty (char *);
extern char *ut_arg_listmember (char *, int, int);

extern void ut_arg_expand (int, char**, int*, char***);

#endif /* UT_ARG_H */
