/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"FileUtils.h"

int
FileExists (char *name)
{
  if (fopen (name, "r") == NULL)
    return 0;
  else
    return 1;
}

void
ErrorMissingFile (char *name)
{
  LineHeader (2);
  printf ("File '%s' not found.\n", name);

  abort ();
}


FILE *
OpenFile (char *name, char *mode)
{
  FILE *file;

  if (mode[0] == 'r')
  {
    LineHeader (0);
    printf ("Parsing file `%s' ...\n", name);
    file = Open4Read (name);
  }
  else if (mode[0] == 'R')
    file = fopen (name, "r");
  else if (mode[0] == 'w')
  {
    LineHeader (0);
    printf ("Writing file `%s' ...\n", name);
    file = Open4Write (name);
  }
  else if (mode[0] == 'W')
    file = fopen (name, "w");
  else if (mode[0] == 'a')
  {
    LineHeader (0);
    printf ("Writing file `%s' ...\n", name);
    file = fopen (name, "a");
  }
  else if (mode[0] == 'A')
    file = fopen (name, "a");
  else
    abort ();

  return file;
}

void
CloseFile (FILE * file, char *name, char *mode)
{
  if (mode[0] == 'r')
  {
    LineHeader (0);
    printf ("Parsed file `%s'.\n", name);
  }
  else if (mode[0] == 'w')
  {
    LineHeader (0);
    printf ("Wrote file `%s'.\n", name);
  }

  fclose (file);
  file = NULL;

  return;
}

/* Open4Read checks if the file exists and opens it.
   Else, the program is aborted.
 */
FILE *
Open4Read (char *filename)
{
  FILE *input = 0;

  /* If the file is not found, the program is aborted. */
  if (fopen (filename, "r") == NULL)
  {
    LineHeader (2);
    printf ("\'%s\' file not found.\n", filename);
    abort ();
  }
  /* Else, the program goes on. */
  else
  {
    input = fopen (filename, "r");
    /*LineHeader(0); */
    /*printf("Parsing file \"%s\"...\n",filename); */
  }

  return input;
}

/* Open4Write opens the file. It it exists, a warning message is
   generated.
 */
FILE *
Open4Write (char *filename)
{
  FILE *input = 0;

  /* If the file still exists, a warning message is generated. */
  if (fopen (filename, "r") != NULL)
  {
    LineHeader (1);
    printf ("Previous file `%s' has been overwritten.\n", filename);
  }

  /* and the program goes on. */
  input = fopen (filename, "w");

  return input;
}

int
NbLines (char *name)
{
  int qty;
  FILE *tmp;
  char command[1000];

  /*
     if(FileExists(name)==0)
     ErrorMissingFile(name);
   */

  sprintf (command, "wc %s > NbL.tmp", name);
  system (command);
  tmp = fopen ("NbL.tmp", "r");
  fscanf (tmp, "%d", &qty);
  fclose (tmp);
  remove ("NbL.tmp");

  return qty;
}

int
NbWords (char *name)
{
  int qty;
  FILE *tmp;
  char command[1000];

  /*
     if(FileExists(name)==0)
     ErrorMissingFile(name);
   */

  sprintf (command, "wc %s > NbL.tmp", name);
  system (command);
  tmp = fopen ("NbL.tmp", "r");
  fscanf (tmp, "%d", &qty);	/* trash */
  fscanf (tmp, "%d", &qty);
  fclose (tmp);
  remove ("NbL.tmp");

  return qty;
}

void
ScanIntVect (int *Array, int dim, FILE * In)
{
  int i;

  In = fopen ("in", "r");

  for (i = 0; i <= dim - 1; i++)
    fscanf (In, "%d", &Array[i]);

  fclose (In);
}

void
ScanFloatVect (float *Array, int dim, FILE * In)
{
  int i;

  In = fopen ("in", "r");

  for (i = 0; i <= dim - 1; i++)
    fscanf (In, "%f", &Array[i]);

  fclose (In);
}

/* char* name instead of FILE* file
 */
void
ScanDoubleVect (double *Array, int dim, char *name)
{
  int i;
  FILE *In;

  In = fopen (name, "r");

  for (i = 0; i <= dim - 1; i++)
    fscanf (In, "%lf", &Array[i]);

  fclose (In);
}
