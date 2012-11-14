/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"DisplayUtils.h"

void
LineHeader (int head)
{
  switch (head)
  {
  case -1:
    printf ("*DEBUG*: ");
    break;
  case 0:
    printf ("Info   : ");
    break;
  case 1:
    printf ("Warning: ");
    break;
  case 2:
    printf ("Error  : ");
    break;
  default:
    printf ("         ");
    break;
  }

  return;
}

void
MessageWNC (int head, int max, char *message)
{
  char *Header = oneDCharDAlloc (10);

  int first = 1;
  int length;
  int line;
  unsigned int m;

  switch (head)
  {
  case 0:
    sprintf (Header, "Info   : ");
    length = strlen ("Info   : ");
    break;
  case 1:
    sprintf (Header, "Warning: ");
    length = strlen ("Warning: ");
    break;
  case 2:
    sprintf (Header, "Error  : ");
    length = strlen ("Error  : ");
    break;
  case -1:
    sprintf (Header, "*DEBUG*: ");
    length = strlen ("*DEBUG*: ");
    break;
  default:
    abort ();
  }

  line = 0;
  m = 0;
  length = 10;
  while (m != strlen (message) - 1)
  {
    line++;
    if (first == 1)
    {
      printf ("%s", Header);
      first = 0;
    }
    else
      printf ("          ");

    Printf1LWNC (message, &m, max - length);
  }

  oneDCharFree (Header);

  return;
}

void
MessageB (int head, int qty, ...)
{
  va_list Parameters;
  int i;

  switch (head)
  {
  case 0:
    printf ("Info   : ");
    break;
  case 1:
    printf ("Warning: ");
    break;
  case 2:
    printf ("Error  : ");
    break;
  case -1:
    printf ("*DEBUG*: ");
    break;
  default:
    printf ("         ");
    break;
  }

  va_start (Parameters, qty);
  for (i = 0; i <= qty - 1; i++)
    printf ("%s", va_arg (Parameters, char *));
  va_end (Parameters);

  return;
}

void
ProgramHeader (char *name, char *version, char *date)
{
  LineHeader (0);
  printf ("------------------------------------------------------------\n");
  LineHeader (0);
  printf ("%s - Version %s\n", name, version);
  LineHeader (0);

  printf ("last modified %s            \n", date);
  /* LineHeader(0); */
  /* printf("                                                       \n"); */
  /* LineHeader(0); */
  /* printf("Hit `man %s` to access the online reference manual. \n",name); */
  /*LineHeader(0);
     printf("                                                       \n");
     LineHeader(0);
     printf("Send comments, requests for help, bugs and suggestions \n");
     LineHeader(0);
     printf("to romain.quey@laposte.net.                          \n"); */
  LineHeader (0);
  printf ("------------------------------------------------------------\n");
  /*printf("\n"); */

  return;
}

void
ProgramHeaderInfo (char *name, char *version, char *date)
{
  LineHeader (0);
  printf ("------------------------------------------------------------\n");
  LineHeader (0);
  printf ("%s - Version %s\n", name, version);
  LineHeader (0);

  printf ("last modified %s            \n", date);
  LineHeader (0);
  printf ("Hit `info %s' to access the on-line reference manual. \n", name);
  /*LineHeader(0);
     printf("                                                       \n");
     LineHeader(0);
     printf("Send comments, requests for help, bugs and suggestions \n");
     LineHeader(0);
     printf("to romain.quey@laposte.net.                          \n"); */
  LineHeader (0);
  printf ("------------------------------------------------------------\n");
  /*printf("\n"); */

  return;
}

void
ProgramHeaderB (char *name, char *version, char *date, int max)
{
  int i;
  char *line = oneDCharDAlloc (max);

  for (i = 0; i <= max - 10; i++)
    line[i] = '-';
  line[max - 10] = '\0';

  LineHeader (0);
  printf ("%s\n", line);

  LineHeader (0);
  printf ("%s - Version %s\n", name, version);
  LineHeader (0);

  printf ("last modified %s            \n", date);
  /* LineHeader(0); */
  /* printf("                                                       \n"); */
  /* LineHeader(0); */
  /* printf("Hit `man %s` to access the online reference manual. \n",name); */
  /*LineHeader(0);
     printf("                                                       \n");
     LineHeader(0);
     printf("Send comments, requests for help, bugs and suggestions \n");
     LineHeader(0);
     printf("to romain.quey@laposte.net.                          \n"); */
  LineHeader (0);
  printf ("%s\n", line);

  oneDCharFree (line);

  return;
}

void
ModuleHeader (char *mname, int fargv, char **fargc, int argv, char **argc)
{
  int i;
  char *fcommand1 = oneDCharDAlloc (1000);
  char *fcommand2 = oneDCharDAlloc (1000);
  char *command1 = oneDCharDAlloc (1000);
  char *command2 = oneDCharDAlloc (1000);

  if (argv == 1)
    sprintf (command1, "[com line] (none)");
  else
    sprintf (command1, "[com line] ");
  sprintf (command2, command1);

  for (i = 1; i < argv; i++)
  {
    sprintf (command2, "%s %s", command1, argc[i]);
    sprintf (command1, "%s", command2);
  }

  if (fargv == 0 || fargv == 1)
    sprintf (fcommand1, "[ini file] (none)");
  else
    sprintf (fcommand1, "[ini file] ");
  sprintf (fcommand2, fcommand1);
  for (i = 1; i < fargv; i++)
  {
    sprintf (fcommand2, "%s %s", fcommand1, fargc[i]);
    sprintf (fcommand1, "%s", fcommand2);
  }

  LineHeader (0);
  printf ("--------------------------------------------------------------\n");
  LineHeader (0);
  printf ("MODULE %3s loaded with arguments:\n", mname);
  MessageWNC (0, 72, fcommand2);
  MessageWNC (0, 72, command2);
  LineHeader (0);
  printf ("--------------------------------------------------------------\n");

  printf ("\n");

  oneDCharFree (fcommand1);
  oneDCharFree (fcommand2);
  oneDCharFree (command1);
  oneDCharFree (command2);

  return;
}

/*
void ProgramNMHeader(char* pname,char* mname,char* version,char* date)
{
    LineHeader(0);
    printf("------------------------------------------------------------\n");
    LineHeader(0);
    printf("%s, module %s - Version %s\n",pname,mname,version);
    LineHeader(0);
    
    printf("last modified %s            \n",date);
    LineHeader(0);
    printf("------------------------------------------------------------\n");
    printf("\n");

    return;
}
*/

void
PercentDisplay (int nb, int tot, int step, int *f)
{
  if (nb >= (double) (*f) * (double) tot / (double) (100 / (double) step))
  {
    LineHeader (0);
    printf ("%3.1d%%\n", step * (*f));
    (*f)++;
  }

  return;
}

/* ReadMan says to read the on-line manual page */
void
ReadMan (char *name)
{
  LineHeader (2);
  printf ("Hit \'man %s\' for more information.\n\n", name);

  return;
}

/* Missing file says file misses and exits the program. */
void
MissingFile (void)
{
  LineHeader (2);
  printf ("Missing file argument!\n\n");
  abort ();

  return;
}

void
BadArguments (void)
{
  LineHeader (2);
  printf ("Bad arguments!\n\n");
  abort ();

  return;
}

void
ArgError (char *arg)
{
  LineHeader (2);
  printf ("Option \'%s\':\n", arg);
  BadArguments ();

  return;
}
