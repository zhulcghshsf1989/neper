/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#ifndef UT_FILE_H
#define UT_FILE_H

extern int ut_file_skip (FILE*, int);
extern int ut_file_nblines (char*);
extern int ut_file_nblines_pointer (FILE *);
extern int ut_file_nbwords (char*);
extern int ut_file_nbwords_test (char*, int);
extern int ut_file_nbwords_testwmessage (char*, int);
extern int ut_file_exist (char*, ...);
extern FILE* ut_file_open (char*, char*);
extern void ut_file_openmessage (char*, char*);
extern void ut_file_close (FILE *, char*, char*);
extern void ut_file_closemessage (char*, char*);
extern int ut_file_skip_char (FILE *, int);
extern int ut_file_tofile (FILE *, int, FILE *, char*, char*);
extern int ut_file_readwcomma (FILE*, double*);
extern int ut_file_format (char*, char**);
extern int ut_file_testformat (char*, char*);
extern int ut_file_nbwords_pointer (FILE *);

extern int ut_file_string_goto (FILE*, char*);
extern int ut_file_string_untilchar (FILE*, char*, char);
extern int ut_file_string_scanncomp (FILE*, char*);
extern int ut_file_string_nextpos (FILE*, char*);
extern int ut_file_string_number (FILE*, char*, char*);

extern int ut_file_thisstring (FILE*, char*);
extern int ut_file_thischar (FILE*, char*);
extern int ut_file_thisint  (FILE*, int*);
extern int ut_file_thisreal (FILE*, double*);
extern int ut_file_nextint  (FILE*, int*);
extern int ut_file_nextchar  (FILE*, char*);
extern int ut_file_nextstring  (FILE*, char*);

extern int ut_file_testpos (FILE*, char*);
extern int ut_file_go (FILE*, char*);

extern int ut_file_line_nbwords_pointer (FILE*, int*);

#endif /* UT_FILE_H */
