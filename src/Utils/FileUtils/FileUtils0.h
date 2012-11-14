/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef FILEUTILS0_H
#define FILEUTILS0_H

extern int FileExists (char *);
extern void ErrorMissingFile (char *);
extern FILE *OpenFile (char *, char *);
extern FILE *Open4Read (char *);
extern FILE *Open4Write (char *);
extern FILE *OPEN4READ (char *);
extern FILE *OpEN4WRITE (char *);
extern int NbLines (char *);
extern int NbWords (char *);
extern void ScanIntVect (int *, int, FILE *);
extern void ScanFloatVect (float *, int, FILE *);
extern void ScanDoubleVect (double *, int, char *);
extern void CloseFile (FILE *, char *, char *);

#endif /* FILEUTILS0_H */
