/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>

#include "../ut_string.h"
#include"../ut.h"
 
int
ut_string_fnr (char *string, char find, char replace)
{
  int i, length, res;

  res = 0;
  length = strlen (string);
  for (i = 0; i <= length - 1; i++)
    if (string[i] == find)
    {
      string[i] = replace;
      res = 1;
    }

  return res;
}
 
int
ut_string_find (char *string, char find)
{
  int i, length, res;

  res = -1;
  length = strlen (string);
  for (i = 0; i <= length - 1; i++)
    if (string[i] == find)
    {
      res = i;
      break;
    }

  return res;
}

int
ut_string_finds (char *string, char* find)
{
  int i, j, equal, res;
  int length  = strlen (string);
  int lengthf = strlen (find);

  res = -1;
  for (i = 0; i < length - lengthf + 1; i++)
  {
    equal = 1;
    for (j = 0; j < lengthf; j++)
    {
      if (string[i + j] != find[j])
      {
	equal = 0;
	break;
      }
    }

    if (equal == 1)
    {
      res = i;
      break;
    }
  }

  return res;
}

int
ut_string_fnrs (char *string, char* find, char* replace, int repeat)
{
  int i, j, p, equal, test = 0;
  int* pos = ut_alloc_1d_int (strlen (string) + 1);
  int length  = strlen (string);
  int lengthf = strlen (find);
  int lengthr = strlen (replace);
  char* string2 = ut_alloc_1d_char (ceil (length * lengthr / lengthf) + 1000);

  while (repeat-- > 0)
  {
    length  = strlen (string);

    pos[0] = 0;

    /* looking for the pos of 'find' in 'string' */
    for (i = 0; i < length - lengthf + 1; i++)
    {
      equal = 1;
      for (j = 0; j < lengthf; j++)
      {
	if (string[i + j] != find[j])
	{
	  equal = 0;
	  break;
	}
      }

      if (equal == 1)
      {
	test = 1;
	pos[++pos[0]] = i;
	i += lengthf - 1;
      }
    }

    if (pos[0] == 0)
      break;
    else
    {
      p = 0;
      for (i = 0; i < length; i++)
      {
	if (ut_array_1d_int_find (pos + 1, pos[0], i) == -1)
	  string2[p++] = string[i];
	else
	{
	  for (j = 0; j < lengthr; j++)
	    string2[p++] = replace[j];

	  i += lengthf - 1;
	}
      }
      string2[p]='\0';
    }
    sprintf (string, "%s", string2);
  }

  ut_free_1d_int (pos);
  ut_free_1d_char (string2);

  return test;
}

int
ut_string_comp (char *in, char **list, int qty, char *out)
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

char *
ut_string_extension (char *in)
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

    ext = ut_alloc_1d_char (NoCext + 1);

    for (i = 0; i <= NoCext - 1; i++)
      ext[i] = in[i + DashPos];
    ext[NoCext] = '\0';
  }

  return ext;
}

char *
ut_string_addextension (char *in, char *ext)
{
  char *out = NULL;

  unsigned int NoCin;		/* nb of characters of in */
  unsigned int NoCext;		/* nb of characters of extin */
  int skip = 0;

  NoCin = strlen (in);
  NoCext = strlen (ext);

  if (ext[0] == '.')
  {
    NoCext--;
    skip++;
  }

  out = ut_alloc_1d_char (NoCin + NoCext + 2);

  /* out = strncpy (out, in, NoCin);
  out = strcat (out, ext + 1); */
  sprintf (out, "%s.%s", in, ext + skip);
  /* out[NoCin + NoCext + skip] = '\0'; */

  return out;
}

/* RAM */
void
ut_string_body (char *in, char* body)
{
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

  for (i = 0; i < NoCbody; i++)
    body[i] = in[i];
  body[NoCbody] = '\0';

  return;
}

char*
ut_string_changeextension (char * string, char * newext)
{
  char* body = ut_alloc_1d_char (strlen (string) + 1);
  char* res;

  ut_string_body (string, body);
  res = ut_string_addextension (body, newext);

  ut_free_1d_char (body);
  
  return res;
}

int
ut_string_untilchar (char *string, char c, int *ppos, char *res)
{
  char *copy;

  if ((unsigned int) *ppos < strlen (string))
  {
    copy = ut_alloc_1d_char (strlen (string) + 1);

    strcpy (copy, string + (*ppos));

    ut_string_fnr (copy, c, '\0');

    strcpy (res, copy);

    *ppos += strlen (copy);
  
    ut_free_1d_char (copy);
  }
  else
    res[0] = '\0';

  if (strlen (res) > 0)
    return 1;
  else
    return 0;
}

int
ut_string_thischarqty (char *string, char c)
{
  int i, res = 0;

  for (i = 0; i < (int) strlen (string); i++)
    if (string[i] == c)
      res++;

  return res;
}

int
ut_string_thischarpos (char *string, char c, int nb)
{
  int pos, tmp, res;

  /* by default, i.e. if nb == 0 or occurrence not found */
  res = -1;

  if (nb != 0)
  {
    tmp = 0;
    for (pos = 0; pos < (int) strlen (string); pos++)
      if (string[pos] == c)
      {
	tmp++;
	if (tmp == nb)		/* the good occurence is found */
	{
	  res = pos;
	  break;
	}
      }
  }

  return res;
}

int
ut_string_stringpos (char *string, char *string2, int nb)
{
  int pos, tmp, res;
  int l1, l2, i, status;

  l1 = (int) strlen (string);
  l2 = (int) strlen (string2);

  /* by default, i.e. if nb == 0 or occurrence not found */
  res = -1;

  if (nb != 0)
  {
    tmp = 0;
    for (pos = 0; pos < l1 - l2 + 1; pos++)
    {
      status = 1;
      for (i = 0; i < l2; i++)
	if (string[pos + i] != string2[i])
	{
	  status = 0;
	  break;
	}
      if (status == 1)
	tmp++;
      if (tmp == nb)		/* the good occurence is found */
      {
	res = pos;
	break;
      }
    }
  }

  return res;
}



void
ut_string_separate (char* string, char c, char*** parts, int* pqty)
{
  int i, pos;
  char** tmp = ut_alloc_2d_char (strlen(string) + 1, strlen(string) + 1);
  char*  val = ut_alloc_1d_char (strlen(string) + 1);
  
  (*pqty) = 0;
  pos = 0;
  while (ut_string_untilchar (string, c, &pos, val))
  {
    pos++;
    sprintf (tmp[(*pqty)++], "%s", val);
  }

  (*parts) = ut_alloc_1d_pchar (*pqty + 1);
  for (i = 0; i < (*pqty); i++)
  {
    (*parts)[i] = ut_alloc_1d_char (strlen (tmp[i]) + 1);
    sprintf ((*parts)[i], "%s", tmp[i]);
  }
  (*parts)[(*pqty)] = NULL;

  ut_free_2d_char (tmp, strlen(string) + 1);
  ut_free_1d_char (val);

  return;
}

void
ut_string_partqty (char* string, char c, int* pqty)
{
  int pos;
  char*  val = ut_alloc_1d_char (strlen(string));
  
  (*pqty) = 0;
  pos = 0;
  while (ut_string_untilchar (string, c, &pos, val) == 1)
  {
    (*pqty)++;
    pos++;
  }

  ut_free_1d_char (val);

  return;
}

int
ut_string_part (char* string, char c, int id, char* part)
{
  int pos, nb, res;
  char* val = ut_alloc_1d_char (strlen(string) + 1);
  
  res = 0;
  nb = 0;
  pos = 0;
  while (ut_string_untilchar (string, c, &pos, val) == 1)
  {
    pos++;
    nb++;
    if (nb == id)
    {
      res = 1;
      break;
    }
  }

  sprintf (part, "%s", val);

  ut_free_1d_char (val);

  return res;
}

/* could be optimized: do not break the list */
int
ut_string_inlist (char* string, char c, char* part)
{
  int res;
  char* tmp = ut_alloc_1d_char (strlen (string) + 3);
  char* tmp2 = ut_alloc_1d_char (strlen (part) + 3);

  sprintf (tmp, "%c%s%c", c, string, c);
  sprintf (tmp2, "%c%s%c", c, part, c);

  res = (strstr (tmp, tmp2) != NULL) ? 1 : 0;

  ut_free_1d_char (tmp);
  ut_free_1d_char (tmp2);

  return res;
}

int
ut_string_nbwords (char* string)
{
  unsigned int i;
  int bef, nbw;


  nbw = 0;
  bef = 0;
  for (i = 0; i < strlen (string); i++)
  {
    if (string[i] != ' ' && string[i] != '\t' && string[i] != '\n')
    {
      if (bef == 0)
      {
	nbw++;
	bef = 1;
      }
    }
    else
      bef = 0;
  }

  return nbw;
}

char*
ut_string_paste (char * string1, char* string2)
{
  char* res;
  
  res = ut_alloc_1d_char (strlen (string1) + strlen (string2) + 1);

  sprintf (res, "%s%s", string1, string2);
  
  return res;
}


double
ut_string_exp_value (char *expr, unsigned int qty, double *d1, double *d2)
{
  unsigned int i;
  int nb;
  double res = 0;
  char expr2[1000];
  char command[1000];
  FILE *file;

  nb = 0;
  for (i = 0; i <= strlen (expr) - 1; i++)
  {
    if (expr[i] != 't')
    {
      expr2[nb] = expr[i];
      nb++;
    }
    else if (expr[i] == 't' &&
	     ((i > 0 && isalpha (expr[i - 1])) || isalpha (expr[i + 1])))
    {
      expr2[nb] = expr[i];
      nb++;
    }
    else
    {
      expr2[nb] = '$';
      nb++;
      expr2[nb] = '1';
      nb++;
    }
  }
  expr2[nb] = '\0';

  /*
     for(i=0;i<=strlen(expr2)-1;i++)
     printf("%c",expr2[i]);
     printf("\n");
   */

  file = ut_file_open ("devtmp", "W");
  for (i = 0; i < qty; i++)
    fprintf (file, "%.12f\n", d1[i]);
  ut_file_close (file, "devtmp", "W");

  sprintf (command, "awk '{print %s}' devtmp > devtmp2", expr2);
  /*printf("command=%s\n",command); */
  system (command);

  file = ut_file_open ("devtmp2", "R");
  for (i = 0; i < qty; i++)
    fscanf (file, "%lf", &d2[i]);
  ut_file_close (file, "devtmp2", "R");

  remove ("devtmp");
  remove ("devtmp2");

  return res;
}

int
ut_char_zero (char c)
{
  if (c == ' ' || c == '\t' || c == '\n')
    return 1;
  else
    return 0;
}

int
ut_string_format (char* in, char* format)
{
  int i;
  int length = strlen (in);
  int dotqty, dotpos, nbqty, isstring;

  nbqty = 0;
  dotqty = 0;
  dotpos = 0;
  isstring = 0;
  for (i = 0; i < length; i++)
  {
    if (in[i] >= '0' && in[i] <= '9')
      nbqty++;
    else if (in[i] == '.')
    {
      dotqty++;
      dotpos = i;
    }
    else
    {
      isstring = 1;
      break;
    }
  }

  if (dotqty > 1)
    isstring = 1;

  // string
  if (isstring == 1)
    sprintf (format, "%%s");
  // int
  else if (dotqty == 0)
  {
    if (in[0] != '0')
      sprintf (format, "%%d");
    else
      sprintf (format, "%%0%dd", length);
  }
  else if (dotqty == 1)
  {
    sprintf (format, "%%0%d.%df", length, length - dotpos - 1);
  }
  else 
    abort ();
  
  return 0;
}

int
ut_string_format_strcmp (char* in, char* format)
{
  int status;
  char* format2 = ut_alloc_1d_char (10);

  ut_string_format (in, format2);

  status = strcmp (format, format2);

  ut_free_1d_char (format2);

  return status;
}
