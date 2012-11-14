/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef CHARUTILS0_H
#define CHARUTILS0_H

extern char *ScanLine (FILE *);
extern void ScanLineB (FILE *, char *, int);
extern int CharScanNComp (char *, FILE *);
extern void Trash (int);
extern void FTrash (int, FILE *);
extern char *CharExtension (char *, char *, char *);
extern char *AddExtension (char *, char *);
extern char *GiveExtension (char *);
extern char *GiveBody (char *);
extern void FPrintfWNC (FILE *, int, int *, int);
extern void FPrintfCharWNC (FILE *, char *, int *, int);
extern void Printf1LWNC (char *, unsigned int *, unsigned int);
extern int NextArgAsInt (char **, int *, char *, int, int);
extern double NextArgAsReal (char **, int *, char *, double, double);
extern char *NextArgAsChar (char **, int *, char *);
extern int StringComp (char *, char **, int, char *);

extern int IsInt (char *);

#endif /* CHARUTILS0_H */
