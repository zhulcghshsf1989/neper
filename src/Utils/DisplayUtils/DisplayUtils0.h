/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#ifndef DISPLAYUTILS0_H
#define DISPLAYUTILS0_H

extern void LineHeader (int);
extern void MessageB (int, int, ...);
extern void MessageWNC (int, int, char *);

extern void ProgramHeader (char *, char *, char *);
extern void ModuleHeader (char *, int, char **, int, char **);
extern void ProgramHeaderB (char *, char *, char *, int);
extern void ProgramHeaderInfo (char *, char *, char *);
extern void ProgramNMHeader (char *, char *, char *, char *);
extern void PercentDisplay (int, int, int, int *);

extern void MissingFile (void);
extern void BadArguments (void);
extern void ArgError (char *);
extern void ReadMan (char *);

#endif /* DISPLAYUTILS0_H */
