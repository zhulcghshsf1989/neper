/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include"ut_file.h"
#include"ut.h"

int
ut_file_nblines (char *filename)
{
  int nblines;
  FILE *file;
  char trash[10000];

  file = fopen (filename, "r");

  nblines = 0;
  while (fgets (trash, 1000, file))
    nblines++;

  fclose (file);

  return nblines;
}

int
ut_file_nblines_pointer (FILE * file)
{
  int nblines;
  char trash[10000];

  nblines = 0;
  while (fgets (trash, 1000, file))
    nblines++;

  fseek (file, 0, 0);

  return nblines;
}

int
ut_file_nbwords (char *filename)
{
  int nbwords;
  FILE *file;
  char trash[10000];

  file = ut_file_open (filename, "R");

  nbwords = 0;
  while (fscanf (file, "%s", trash) == 1)
    nbwords++;

  ut_file_close (file, filename, "R");

  return nbwords;
}

int
ut_file_nbwords_test (char* filename, int qty)
{
  return ut_file_nbwords (filename) - qty;
}

int
ut_file_nbwords_testwmessage (char* filename, int qty)
{
  if (ut_file_nbwords (filename) != qty)
  {
    ut_print_message (2, 0, "Wrong number of data in input file %s.\n", filename);
    abort ();
  }

  return 0;
}

int
ut_file_nbwords_pointer (FILE * file)
{
  int nbwords;
  char trash[10000];

  nbwords = 0;
  while (fscanf (file, "%s", trash) == 1)
    nbwords++;

  fseek (file, 0, 0);

  return nbwords;
}

int
ut_file_exist (char *name, ...)
{
  int res;
  va_list args;
  char* fullname = ut_alloc_1d_char (1000);
  FILE* file;

  va_start (args, name);
  vsprintf (fullname, name, args);

  file = fopen (fullname, "r");
  if (file == NULL)
    res = 0;
  else
  {
    fclose (file);
    res = 1;
  }

  ut_free_1d_char (fullname);
  va_end (args);

  return res;
}

FILE *
ut_file_open (char *name, char *mode)
{
  FILE *file;

  ut_file_openmessage (name, mode);

  if (mode[0] == 'r' || mode[0] == 'R')
  {
    file = fopen (name, "r");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s: no such file.\n", name);
      abort ();
    }
  }
  else if (mode[0] == 'w' || mode[0] == 'W')
  {
    file = fopen (name, "w");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s cannot be opened for writing.\n", name);
      abort ();
    }
  }
  else if (mode[0] == 'a' || mode[0] == 'A')
  {
    file = fopen (name, "a");
    if (file == NULL)
    {
      ut_print_lineheader (2);
      printf ("%s cannot be opened for writing.\n", name);
      abort ();
    }
  }
  else
    abort ();

  return file;
}

void
ut_file_openmessage (char *name, char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsing file `%s' ...\n", name);
  }
  else if (mode[0] == 'w')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s' ...\n", name);
  }
  else if (mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s' ...\n", name);
  }

  return;
}

void
ut_file_testpath (char *name, char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsing file `%s' ...\n", name);
  }
  else if (mode[0] == 'w')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s' ...\n", name);
  }
  else if (mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Writing file `%s' ...\n", name);
  }

  return;
}


void
ut_file_close (FILE * file, char *name, char *mode)
{
  ut_file_closemessage (name, mode);

  fclose (file);
  file = NULL;

  return;
}

void
ut_file_closemessage (char *name, char *mode)
{
  if (mode[0] == 'r')
  {
    ut_print_lineheader (0);
    printf ("    [i] Parsed file `%s'.\n", name);
  }
  else if (mode[0] == 'w' || mode[0] == 'a')
  {
    ut_print_lineheader (0);
    printf ("    [o] Wrote file `%s'.\n", name);
  }

  return;
}

int
ut_file_skip (FILE* file, int qty)
{
  int i, status;
  char trash[1000];

  if (qty == 0)
    return 0;
  else if (qty > 0)
    for (i = 0; i < qty; i++)
    {
      if (fscanf (file, "%s", trash) != 1)
	return -1;
    }
  else
    for (i = 0; i > qty; i--)
    {
      do
      {
	fseek (file, -1, SEEK_CUR);
	status = ut_file_testpos (file, "wb");
      }
      while (status != 1);
    }

  return 0;
}

int
ut_file_skip_char (FILE * file, int qty)
{
  int i;
  char trash;

  if (qty == 0)
    return 0;
  else if (qty > 0)
    for (i = 0; i < qty; i++)
      if (fscanf (file, "%c", &trash) != 1)
	return -1;
  else
    abort ();

  return 0;
}

int
ut_file_tofile (FILE * in, int qty, FILE * out, char *a, char *b)
{
  int i;
  char s[1000];

  for (i = 0; i < qty; i++)
  {
    if (fscanf (in, "%s", s) != 1)
      return -1;
    fprintf (out, "%s%s", s, a);
  }
  fprintf (out, "%s", b);

  return 0;
}


int
ut_file_readwcomma (FILE* file, double* pres)
{
  char *tmp = ut_alloc_1d_char (1000);

  int status = 0;
  if (fscanf (file, "%s", tmp) == 1)
    (*pres) = ut_num_readwcomma (tmp);
  else
    status = -1;

  ut_free_1d_char (tmp);

  return status;
}

int
ut_file_testformat (char* filename, char* format)
{
  int res;
  char* format2 = NULL;

  ut_file_format (filename, &format2);
  
  if (strcmp (format, format2) == 0)
    res = 1;
  else 
    res = 0;

  ut_free_1d_char (format2);

  return res;
}

int
ut_file_format (char* filename, char** pformat)
{
  int res, status;
  FILE* file = ut_file_open (filename, "R");
  char* string = ut_alloc_1d_char (1000);

  status = fscanf (file, "%s", string);

  if (status != 1)
  {
    (*pformat) = NULL;
    res = -1;
  }
  else if (strncmp (string, "***tess", 7) == 0)
  {
    (*pformat) = ut_alloc_1d_char (5);
    sprintf ((*pformat), "tess");
    res = 1;
  }
  else if (strncmp (string, "***mtess", 8) == 0)
  {
    (*pformat) = ut_alloc_1d_char (6);
    sprintf ((*pformat), "mtess");
    res = 1;
  }
  else if (strcmp (string, "***vox") == 0)
  {
    (*pformat) = ut_alloc_1d_char (4);
    sprintf ((*pformat), "vox");
    res = 1;
  }
  else if (strcmp (string, "$MeshFormat") == 0)
  {
    (*pformat) = ut_alloc_1d_char (9);
    sprintf ((*pformat), "gmsh_msh");
    res = 1;
  }
  else if (strcmp (string, "***geometry") == 0)
  {
    (*pformat) = ut_alloc_1d_char (12);
    sprintf ((*pformat), "zebulon_geof");
    res = 1;
  }
  else
    res = -1;

  ut_file_close (file, filename, "R");
  
  ut_free_1d_char (string);

  return res;
}


// does not take the blank characters into account (because fscanf skip
// them)
int
ut_file_string_goto (FILE * file, char* string)
{
  int status;
  unsigned int j;
  char c;
  fpos_t pos;
  int match;

  do
  {
    fgetpos (file, &pos);
    match = 1;
    for (j = 0; j < strlen (string); j++)
    {
      status = fscanf (file, "%c", &c);
      if (status != 1 || c != string[j])
      {
	match = 0;
	break;
      }
    }
    fsetpos (file, &pos);
  }
  while (match == 0 && fscanf (file, "%c", &c) != EOF);

  return match;
}

int
ut_file_string_untilchar (FILE * file, char* string, char c)
{
  int i;

  if (fscanf (file, "%c", &(string[0])) != 1)
    return -1;

  for (i = 1; ; i++)
  {
    if (fscanf (file ,"%c", string + i) != 1)
      return -1;

    if (string[i] == c)
      break;
  }
  string[i] = '\0';

  return 0;
}

int
ut_file_string_scanncomp (FILE * file, char* string)
{
  int status;
  char* scan = ut_alloc_1d_char (1000);

  status = fscanf (file, "%s", scan);

  if (status == 1)
    status = strcmp (string, scan);
  else
    status = -1;

  ut_free_1d_char (scan);

  return status;
}


int
ut_file_string_nextpos (FILE * file, char* string)
{
  int p, ok;
  char* tmp = ut_alloc_1d_char (1000);
  fpos_t pos;

  fgetpos (file, &pos);

  ok = 0;
  p = 0;
  while (fscanf (file, "%s", tmp) != EOF)
  {
    p++;
    if (strcmp (tmp, string) == 0)
    {
      p--;
      ok = 1;
      break;
    }
  }
  fprintf (file, "\n");

  fsetpos (file, &pos);

  ut_free_1d_char (tmp);

  return (ok == 1)? p:-1;
}

int
ut_file_string_number (FILE * file, char* string, char* opt)
{
  int nb;
  char* tmp = ut_alloc_1d_char (1000);
  fpos_t pos;

  fgetpos (file, &pos);

  nb = 0;
  while (fscanf (file, "%s", tmp) != EOF)
    if (strcmp (tmp, string) == 0)
    {
      if (strcmp (opt, "lb") == 0 || strcmp (opt, "lB") == 0)
	fseek (file, -strlen (string), SEEK_CUR);

      if (strlen (opt) != 0)
      {
	if (ut_file_testpos (file, opt) == 1)
	  nb++;
      }
      else
	nb++;
      
      if (strcmp (opt, "lb") == 0 || strcmp (opt, "lB") == 0)
	fseek (file, strlen (string), SEEK_CUR);
    }

  fsetpos (file, &pos);

  ut_free_1d_char (tmp);

  return nb;
}

int
ut_file_thisstring (FILE * file, char* string)
{
  fpos_t pos;

  fgetpos (file, &pos);

  if (fscanf (file, "%s", string) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_thisint (FILE * file, int* pval)
{
  fpos_t pos;

  fgetpos (file, &pos);

  if (fscanf (file, "%d", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_thischar (FILE * file, char* pval)
{
  fpos_t pos;

  fgetpos (file, &pos);

  if (fscanf (file, "%c", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_thisreal (FILE * file, double* pval)
{
  fpos_t pos;

  fgetpos (file, &pos);

  if (fscanf (file, "%lf", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_testpos (FILE* file, char* p)
{
  int fsstatus, status, res, qty;
  char c, c2;
  fpos_t pos;

  fgetpos (file, &pos);

  res = 0;
  if (strcmp (p, "le") == 0)
  {
    do
      status = fscanf (file, "%c", &c);
    while (c == ' ' || c == '\t');

    if (status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "lE") == 0)
  {
    status = fscanf (file, "%c", &c);

    if (status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "lB") == 0)
  {
    fsstatus = fseek (file, -1, SEEK_CUR);
    status = fscanf (file, "%c", &c);

    if (fsstatus == -1 || status != 1 || (status == 1 && c == '\n'))
    res = 1;
  }
  else if (strcmp (p, "lb") == 0)
  {
    qty = 0;
    do
    {
      fsstatus = fseek (file, --qty, SEEK_CUR);
      status = fscanf (file, "%c", &c);
    }
    while (c == ' ' || c == '\t');

    if (fsstatus == - 1 || status != 1 || (status == 1 && c == '\n'))
      res = 1;
  }
  else if (strcmp (p, "wb") == 0)
  {
    if (ftell (file) == 0)
      c = ' ';
    else
    {
      fseek (file, -1, SEEK_CUR);
      status = fscanf (file, "%c", &c);
    }
    status = fscanf (file, "%c", &c2);

    if (status == 1
     && (c == ' ' || c == '\t' || c == '\n')
     && ! (c2 == ' ' || c2 == '\t' || c2 == '\n'))
      res = 1;
  }
  else if (strcmp (p, "we") == 0)
  {
    status = fscanf (file, "%c%c", &c, &c2);

    if (status == 2
     && ! (c == ' ' || c == '\t' || c == '\n')
     && (c2 == ' ' || c2 == '\t' || c2 == '\n'))
      res = 1;
  }
  else if (strcmp (p, "fe") == 0)
  {
    status = fscanf (file, "%c", &c);

    if (status != 1)
      res = 1;
  }
  else
    abort ();

  fsetpos (file, &pos);

  return res;
}

int
ut_file_nextint (FILE * file, int* pval)
{
  int tmp;
  fpos_t pos;

  fgetpos (file, &pos);
  if (fscanf (file, "%d", &tmp) != 1)
    return -1;

  if (fscanf (file, "%d", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_nextchar (FILE * file, char* pval)
{
  char tmp;
  fpos_t pos;

  fgetpos (file, &pos);
  if (fscanf (file, "%c", &tmp) != 1)
    return -1;

  if (fscanf (file, "%c", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_nextstring (FILE * file, char* pval)
{
  fpos_t pos;

  fgetpos (file, &pos);
  if (fscanf (file, "%s", pval) != 1)
    return -1;
  else
  {
    fsetpos (file, &pos);
    return 1;
  }
}

int
ut_file_line_nbwords_pointer (FILE* file, int* pqty)
{
  int status;
  char* trash = ut_alloc_1d_char (10000);
  fpos_t pos;

  fgetpos (file, &pos);

  trash = fgets (trash, 10000, file);

  status = 0;
  if (strlen (trash) == 9999)
    status = -1;
  
  fsetpos (file, &pos);

  (*pqty) = ut_string_nbwords (trash);

  return status;
}

/*
int
ut_file_go (FILE* file, char* go)
{
  char c;
  int i, qty, status;
  char* goo = ut_alloc_1d_char (strlen (go) + 1);

  if (sscanf (go, "%d%s", &qty, goo) != 2)
  {
    qty = 1;
    goo = go;
  }

  for (i = 0; i < qty; i++)
  {
    if (strcmp (goo, "h") == 0)
    {
      fseek (file, -1, SEEK_CUR);
      status = ut_file_thischar (file, &c);
    }
    else if (strcmp (goo, "l") == 0)
    {
      fseek (file,  1, SEEK_CUR);
      status = ut_file_thischar (file, &c);
    }
    else if (strcmp (goo, "W") == 0)
    {
      do
      {
	fscanf (file, "%c", &c);
	status = ut_file_thischar (file, &c);
	printf ("continued: status = %d c = %c\n", status, c);
      }
      while (status != -1 && (c == ' ' || c == '\t' || c == '\n'));
      printf ("stopped: status = %d c = %c\n", status, c);
    }
    else if (strcmp (goo, "E") == 0)
    {
      do
      {
	fscanf (file, "%c", &c);
	status = ut_file_thischar (file, &c);
	printf ("continued: status = %d c = %c\n", status, c);
      }
      while (status != -1 && (c == ' ' || c == '\t' || c == '\n'));
      printf ("stopped: status = %d c = %c\n", status, c);
    }
    else
      abort ();
  }

  return (status == -1) ? -1:0;
}
*/
