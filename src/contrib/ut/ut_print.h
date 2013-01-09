/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_PRINT_H
#define UT_PRINT_H

extern void ut_print_wnc_int (FILE *, int, int *, int);
extern void ut_print_wnc_int_header (FILE *, int, int *, int, char*);
extern int  ut_print_wnc_char (FILE *, char *, int *, int);
extern void ut_print_wncall_char (FILE *, char *, int *, int);
extern void ut_print_wnc (FILE*, int*, int, char*, ...);
extern void ut_print_wncall (FILE*, int*, int, char*, ...);
extern void ut_print_wnc_wncall (FILE*, int*, int, int*, int, char*, ...);
extern void ut_print_1lwnc (FILE *, char *, unsigned int *, unsigned int);
extern void ut_print_lineheader (int);
extern void ut_print_message (int, int, char *, ...);
extern void ut_print_messagewnc (int, int, char *);
extern void ut_print_missingfile (char *);
extern void ut_print_badfileformat (char *);
extern void ut_print_moduleheader (char *, int, char **, int, char **);
extern void ut_print_progress (FILE*, long, long, char*, char*);
extern void ut_print_progress_nonl (FILE*, long, long, char*, char*);
extern void ut_print_reportbug (void);
extern void ut_print_gplv3 (FILE*);
extern void ut_error_reportbug (void);
extern void ut_print_clearline (FILE* file, int nb);

#endif /* UT_PRINT_H */
