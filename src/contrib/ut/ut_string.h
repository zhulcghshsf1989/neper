/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_STRING_H
#define UT_STRING_H

extern int ut_string_fnr (char *, char, char);
extern char* ut_string_paste (char *, char*);
extern char* ut_string_array_paste (char **, int);
extern char* ut_string_array_paste_cmp (char** string, int qty);
extern int ut_string_find (char*, char);
extern int ut_string_finds (char*, char*);
extern int ut_string_fnrs (char *, char*, char*, int);
extern int ut_string_comp (char *, char **, int, char *);
extern void ut_string_body (char*, char*);
extern char* ut_string_changeextension (char *, char *);
extern char* ut_string_extension (char *);
extern char* ut_string_addextension (char*, char*);
extern char* ut_string_stc (char *, char, char *);
extern int ut_string_untilchar (char *, char, int *, char *);
extern int ut_string_thischarqty (char *, char);
extern int ut_string_thischarpos (char *, char, int);
extern int ut_string_stringpos (char *, char*, int);
extern void ut_string_separate (char*, char, char***, int*);
extern int ut_string_inlist (char*, char, char*);
extern void ut_string_partqty (char*, char, int*);
extern int ut_string_part (char*, char, int, char*);
extern int ut_string_nbwords (char*);
extern double ut_string_exp_value (char*, unsigned int, double*, double*);
extern int ut_string_format (char*, char*);
extern int ut_string_format_strcmp (char*, char*);
extern int ut_string_memcpy (char*, char**);

#endif /* UT_STRING_H */
