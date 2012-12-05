/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"ut_arg.h"
#include"ut.h"

int
ut_arg_nextasint (char **argv, int *pi, char *option, int min, int max)
{
  int res;
  int length;
  int i;

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is an int */
  if (sscanf (argv[*pi], "%d", &res) == 0)
    ut_arg_error (option, "not an integer value.");

  length = strlen (argv[*pi]);
  for (i = 0; i <= length - 1; i++)
    if (argv[*pi][i] == '.')
      ut_arg_error (option, "not an integer value.");

  /* checking bounds */
  if (res < min || res > max)
    ut_arg_error (option, "bad integer value.");

  return res;
}

int
ut_arg_nextaslogical (char **argv, int *pi, char *option)
{
  return ut_arg_nextasint (argv, pi, option, 0, 1);
}


/* Same than NextArgAsInt but for reals. */
double
ut_arg_nextasreal (char **argv, int *pi, char *option, double min, double max)
{
  double res;
  double eps = 1e-15;

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is a real */
  if (sscanf (argv[*pi], "%lf", &res) == 0)
    ut_arg_error (option, "not a real value.");

  if (res < min - eps || res > max + eps)
  {
    printf ("min = %f max = %f\n", min, max);
    ut_arg_error (option, "bad real value.");
  }

  return res;
}

/* Same than NextArgAsInt but for chars. */
char *
ut_arg_nextaschar (char **argv, int *pi, char *option)
{
  char *res = ut_alloc_1d_char (1000);

  /* incrementing i */
  (*pi)++;

  /* reading the argument and testing if it is an int */
  if (sscanf (argv[*pi], "%s", res) == 0)
    ut_arg_error (option, "not a char string value.");

  /*if(res<min-eps || res>max+eps) */
  /*ut_arg_error(option); */

  return res;
}


void
ut_arg_badarg (void)
{
  ut_print_lineheader (2);
  printf ("Bad arguments!\n\n");
  abort ();

  return;
}

void
ut_arg_error (char *arg, char* explaination)
{
  ut_print_lineheader (2);
  printf ("argument \'%s\': %s\n", arg, explaination);
  abort ();

  return;
}

int
ut_arg_listqty (char *arg)
{
  return 1 + ut_string_thischarqty (arg, ',');
}

char *
ut_arg_listmember (char *arg, int qty, int nb)
{
  int i, pos1, pos2, length;
  char *res;

  /* searching pos1, the character/comma before the arg */
  pos1 = ut_string_thischarpos (arg, ',', nb - 1);

  /* searching pos2, the character/comma after the arg */
  if (nb < qty)
    pos2 = ut_string_thischarpos (arg, ',', nb);
  else
    pos2 = strlen (arg);

  length = pos2 - pos1 - 1;
  res = ut_alloc_1d_char (length + 1);
  for (i = 0; i < length; i++)
    res[i] = arg[pos1 + i + 1];
  res[length] = '\0';

  return res;
}

void
ut_arg_expand (int argc, char** argv, int* pexpargc, char*** pexpargv)
{
  int i, ibeg, iend, j;
  char* var = NULL;
  char* nextvar = NULL;
  char* prevvar = NULL;
  double value, beg, stp, end, eps;
  char* tmp = ut_alloc_1d_char (1000);
  char* tmp2 = ut_alloc_1d_char (1000);
  char* tmp3 = ut_alloc_1d_char (1000);
  char* tmp4 = ut_alloc_1d_char (1000);
  char* format = ut_alloc_1d_char (10);

  (*pexpargc) = 0;
  (*pexpargv) = ut_alloc_1d_pchar (1);

  for (i = 0; i < argc; i++)
  {
    if (strcmp (argv[i], "-loop") == 0)
    {
      var = ut_alloc_1d_char (strlen (argv[++i]) + 1);
      sscanf (argv[i],   "%s", var);
      sscanf (argv[++i], "%lf", &beg);
      ut_string_format (argv[++i], format);
      sscanf (argv[i], "%lf", &stp);
      sscanf (argv[++i], "%lf", &end);
      eps = stp * 1e-3;

      prevvar = ut_alloc_1d_char (strlen (var) + 5);
      sprintf (prevvar, "PREV%s", var);
      nextvar = ut_alloc_1d_char (strlen (var) + 5);
      sprintf (nextvar, "NEXT%s", var);

      ibeg = ++i;
      iend = argc - 1;

      for (j = ibeg; j < argc; j++)
	if (strcmp (argv[j], "-endloop") == 0)
	{
	  iend = j - 1;
	  break;
	}

      for (value = beg; value <= end + eps; value += stp)
      {
	for (i = ibeg; i <= iend; i++)
	{
	  sscanf (argv[i], "%s", tmp);

	  if (format[strlen (format) - 1] == 'f')
	  {
	    sprintf (tmp2, format, value);
	    sprintf (tmp3, format, value - stp);
	    sprintf (tmp4, format, value + stp);
	  }
	  else if (format[strlen (format) - 1] == 'd')
	  {
	    sprintf (tmp2, format, ut_num_d2ri (value));
	    sprintf (tmp3, format, ut_num_d2ri (value - stp));
	    sprintf (tmp4, format, ut_num_d2ri (value + stp));
	  }

	  ut_string_fnrs (tmp, prevvar, tmp3, 100);
	  ut_string_fnrs (tmp, nextvar, tmp4, 100);
	  ut_string_fnrs (tmp, var, tmp2, 100);

	  (*pexpargc)++;
	  (*pexpargv) = ut_realloc_1d_pchar ((*pexpargv), (*pexpargc));
	  (*pexpargv)[(*pexpargc) - 1]
	    = ut_alloc_1d_char (strlen (tmp) + 1);
	  sprintf ((*pexpargv)[(*pexpargc) - 1], "%s", tmp);
	}
      }
      i++; // skipping -endloop
    }
    else
    {
      (*pexpargc)++;
      (*pexpargv) = ut_realloc_1d_pchar ((*pexpargv), (*pexpargc));
      (*pexpargv)[(*pexpargc) - 1]
	= ut_alloc_1d_char (strlen (argv[i]) + 1);
      sprintf ((*pexpargv)[(*pexpargc) - 1], "%s", argv[i]);
    }
  }

  ut_free_1d_char (tmp);
  ut_free_1d_char (tmp2);
  ut_free_1d_char (tmp3);
  ut_free_1d_char (tmp4);
  ut_free_1d_char (format);

  return;
}
