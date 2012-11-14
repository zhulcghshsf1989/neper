/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"CharUtils.h"
#include<ctype.h>

extern char *
ScanLine (FILE * in)
{
  int max = 1000;
  unsigned int length;
  char *string;
  char *string2;

  string = oneDCharDAlloc (max);
  string2 = oneDCharDAlloc (max);

  fgets (string, max, in);
  length = strlen (string) - 1;

  string2 = strncpy (string2, string, length);

  oneDCharFree (string);

  return string2;
}

extern void
ScanLineB (FILE * in, char *string2, int max)
{
  unsigned int length;
  char *string;

  string = oneDCharDAlloc (max);

  fgets (string, max, in);
  length = strlen (string) - 1;

  string2 = strncpy (string2, string, length);

  oneDCharFree (string);

  return;
}

void
Trash (int qty)
{
  int i;
  char trash[1000];

  for (i = 1; i <= qty; i++)
    scanf ("%s", trash);

  return;
}

void
FTrash (int qty, FILE * file)
{
  int i;
  char trash[1000];

  for (i = 1; i <= qty; i++)
    fscanf (file, "%s", trash);

  return;
}

int
CharScanNComp (char *string, FILE * file)
{
  char *scan;
  int res;

  scan = oneDCharDAlloc (1000);

  fscanf (file, "%s", scan);

  res = strcmp (string, scan);

  oneDCharFree (scan);

  return res;
}

/* This routine returns a string which is "in-extin+extout" if in
 * contains extin, "in+extout" else.
 * in="export.tess", extin=".tess", extout=".geo" -> out="export.geo"
 * in="export",      extin=".tess", extout=".geo" -> out="export.geo"
 * in="export.ddd",  extin=".tess", extout=".geo" -> out="export.ddd.geo"
 */
char *
CharExtension (char *in, char *extin, char *extout)
{
  char *out;

  unsigned int NoCin;		/* nb of characters of in */
  unsigned int NoCextin;	/* nb of characters of extin */
  unsigned int NoCextout;	/* nb of characters of extout */

  NoCin = strlen (in);
  NoCextin = strlen (extin);
  NoCextout = strlen (extout);

  if (strcmp (in + NoCin - NoCextin, extin) == 0)
  {
    out = oneDCharDAlloc (NoCin - NoCextin + NoCextout + 1);
    out = strncpy (out, in, NoCin - NoCextin);
    out = strcat (out, extout);
    out[NoCin - NoCextin + NoCextout] = '\0';
  }
  else
  {
    out = oneDCharDAlloc (NoCin + NoCextout + 1);
    out = strcpy (out, in);
    out = strcat (out, extout);
    out[NoCin + NoCextout] = '\0';
  }

  return out;
}

char *
AddExtension (char *in, char *ext)
{
  char *out;

  unsigned int NoCin;		/* nb of characters of in */
  unsigned int NoCext;		/* nb of characters of extin */

  NoCin = strlen (in);
  NoCext = strlen (ext);

  out = oneDCharDAlloc (NoCin + NoCext + 1);

  out = strncpy (out, in, NoCin);
  out = strcat (out, ext);
  out[NoCin + NoCext] = '\0';

  return out;
}


char *
GiveExtension (char *in)
{
  char *ext;

  int i;
  int NoCin, NoCext;
  int DashPos = -1;

  NoCin = strlen (in);

  for (i = NoCin - 1; i >= 0; i--)
  {
    if (in[i] == '.')
    {
      DashPos = i;
      break;
    }
  }

  if (DashPos == -1)
    ext = NULL;
  else
  {
    NoCext = NoCin - DashPos;

    ext = oneDCharDAlloc (NoCext + 1);

    for (i = 0; i <= NoCext - 1; i++)
      ext[i] = in[i + DashPos];
    ext[NoCext] = '\0';
  }

  return ext;
}

char *
GiveBody (char *in)
{
  char *body;

  int i;
  int NoCin, NoCbody;
  int DashPos = -1;

  NoCin = strlen (in);

  for (i = NoCin - 1; i >= 0; i--)
  {
    if (in[i] == '.')
    {
      DashPos = i;
      break;
    }
  }

  if (DashPos == -1)
    NoCbody = NoCin;
  else
    NoCbody = DashPos;

  body = oneDCharDAlloc (NoCbody + 1);

  for (i = 0; i <= NoCbody - 1; i++)
    body[i] = in[i];
  body[NoCbody] = '\0';

  return body;
}

/* FPrintfWNC writes integer nb into file on a maximum of 72 columns. */
extern void
FPrintfWNC (FILE * file, int nb, int *pnbcol, int nbcolmax)
{
  int length = IntTenLen (nb);

  if (length > nbcolmax)
  {
    ut_print_message (2, 0,
		      "The max. number of columns of the output file is too low!\n");
    abort ();
  }

  if ((*pnbcol) == 0)
  {
    fprintf (file, "%d", nb);
    (*pnbcol) = length + 1;
  }
  else if ((*pnbcol) + 2 + length <= nbcolmax)
  {
    fprintf (file, " %d", nb);
    (*pnbcol) += length + 2;
  }
  else
  {
    fprintf (file, "\n%d", nb);
    (*pnbcol) = length + 1;
  }

  return;
}

/* FPrintfWNC writes integer nb into file on a maximum of 72 columns. */
extern void
FPrintfCharWNC (FILE * file, char *string, int *pnbcol, int nbcolmax)
{
  int length = strlen (string);

  if (length > nbcolmax)
  {
    ut_print_message (2, 0,
		      "The max. number of columns of the output file is too low!\n");
    abort ();
  }

  if ((*pnbcol) == 0)
  {
    fprintf (file, "%s", string);
    (*pnbcol) = length + 1;
  }
  else if ((*pnbcol) + 2 + length <= nbcolmax)
  {
    fprintf (file, " %s", string);
    (*pnbcol) += length + 2;
  }
  else
  {
    fprintf (file, "\n%s", string);
    (*pnbcol) = length + 1;
  }

  return;
}

/* Printf1LWNC prints the beginning of char*, until nbclomax is reached. */
/* (no word is cut.) It returns the remaining char*. */
extern void
Printf1LWNC (char *string, unsigned int *pnbcol, unsigned int nbcolmax)
{
  int i;
  int LastSpace = 0;

  while (string[(*pnbcol)] == ' ')
    (*pnbcol)++;

  /*  Looking for the last space character */
  if (strlen (string) <= nbcolmax + (*pnbcol))
    LastSpace = strlen (string);
  else
    for (i = nbcolmax + (*pnbcol); i >= 0; i--)
      if (string[i] == ' ')
      {
	LastSpace = i;
	break;
      }

  /*printf("LastSpace=%d\n",LastSpace); */

  /* Is there a end line character (\n)? */
  /*
     for(i=LastSpace-1;i>=0;i--)
     if(string[i]=='\n')
     {
     LastSpace=i;
     break;
     }

     printf("LastSpace=%d\n",LastSpace);
   */

  for (i = (*pnbcol); i <= LastSpace - 1; i++)
    printf ("%c", string[i]);
  printf ("\n");

  /*
     if(string[LastSpace]=='\n')
     LastSpace+=2;
   */

  (*pnbcol) = IntMin (LastSpace, strlen (string) - 1);

  return;
}

/* NextArgAsInt increments *pi and check if argv[*pi] is an integer. */
/* if yes, and if it bw min and max (strictly), it is returned; */
/* else an error message is generated. */
int
NextArgAsInt (char **argv, int *pi, char *option, int min, int max)
{
  int res;
  int length;
  int i;

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is an int */
  if (sscanf (argv[*pi], "%d", &res) == 0)
    ArgError (option);

  length = strlen (argv[*pi]);
  for (i = 0; i <= length - 1; i++)
    if (argv[*pi][i] == '.')
      ArgError (option);

  /* checking bounds */
  if (res < min || res > max)
    ArgError (option);

  return res;
}

/* Same than NextArgAsInt but for reals. */
double
NextArgAsReal (char **argv, int *pi, char *option, double min, double max)
{
  double res;
  double eps = 1e-15;

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is an int */
  if (sscanf (argv[*pi], "%lf", &res) == 0)
    ArgError (option);

  if (res < min - eps || res > max + eps)
    ArgError (option);

  return res;
}

/* Same than NextArgAsInt but for chars. */
char *
NextArgAsChar (char **argv, int *pi, char *option)
{
  char *res = oneDCharDAlloc (1000);

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is an int */
  if (sscanf (argv[*pi], "%s", res) == 0)
    ArgError (option);

  /*if(res<min-eps || res>max+eps) */
  /*ArgError(option); */

  return res;
}

int
StringComp (char *in, char **list, int qty, char *out)
{
  int i;
  int res = 0;

  /* Checking if it matches exactly */
  for (i = 1; i <= qty; i++)	/* for every list parameter */
    if (strcmp (in, list[i]) == 0)
    {
      sprintf (out, "%s", list[i]);
      return 0;
    }

  for (i = 1; i <= qty; i++)	/* for every list parameter */
  {
    /* else, checking if one possibility could be ok */
    if (strncmp (in, list[i], strlen (in)) == 0)
    {
      if (res == 0)		/* this is a good possibility. */
	res = i;
      else			/* ie there are several possibilities: nok */
	return 1;
    }
  }

  if (res != 0)
  {
    sprintf (out, "%s", list[res]);	/* this is the goo possibility. */
    return 0;
  }
  else
    return -1;			/* there is no possibility. */
}

int
IsInt (char *string)
{
  int trash;
  int res;

  if (sscanf (string, "%d", &trash) == 0)
    res = 0;
  else
    res = 1;

  return res;
}
