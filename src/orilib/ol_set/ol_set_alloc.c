/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/
#include "ol_set_alloc.h"

struct OL_SET
ol_set_alloc (size_t size)
{
  unsigned int i;
  struct OL_SET Set;

  Set.size = size;
  Set.q = ut_alloc_2d (size, 4);
  Set.weight = ut_alloc_1d (size);
  for (i = 0; i < size; i++)
    Set.weight[i] = 1;

  return Set;
}

void
ol_set_free (struct OL_SET Set)
{
  ut_free_2d (Set.q, Set.size);
  ut_free_1d (Set.weight);

  return;
}

int
ol_set_fscanf (FILE * file, struct OL_SET *pSet, int *pf)
{
  size_t i;
  double *e = ol_e_alloc ();
  unsigned int size;
  int test, firstline_nbw, whole_nbw;

  // checking the number of string on the first line
  // can be 1, 3 or 4. 
  firstline_nbw = ut_file_line_nbwords_pointer (file);
  whole_nbw = ut_file_nbwords_pointer (file);

  /* required because size_t cannot be scanf (???) neither
     with %u nor with %zu with -pedantic (ISO C90) */
  /* adding 'z' is suggested on
     http://manpages.ubuntu.com/manpages/jaunty/man3/scanf.3.html */

  if (firstline_nbw == 1)
  {
    test = fscanf (file, "%u", &size);
    (*pSet).size = (size_t) size;
    if (pf != NULL)
      (*pf) = 3;

    if (test == -1)
      return 0;
  }
  else if (firstline_nbw == 3 || firstline_nbw == 4)
    (*pSet).size = whole_nbw / firstline_nbw;
  else
    return 0;

  if (pf != NULL)
  {
    if (firstline_nbw == 3)
      (*pf) = 1;
    else if (firstline_nbw == 4)
      (*pf) = 2;
  }

  (*pSet) = ol_set_alloc ((*pSet).size);

  for (i = 0; i < (*pSet).size; i++)
  {
    ol_e_fscanf (file, e);
    ol_e_q (e, (*pSet).q[i]);

    if (firstline_nbw != 3)
      fscanf (file, "%lf", &((*pSet).weight[i]));
  }

  ol_e_free (e);

  return 1;
}

int
ol_set_fscanf_short (FILE * file, struct OL_SET *pSet)
{
  size_t i;
  int test = 0;
  double *e = ol_e_alloc ();

  (*pSet).size = ut_file_nblines_pointer (file);

  (*pSet) = ol_set_alloc ((*pSet).size);

  for (i = 0; i < (*pSet).size; i++)
  {
    test = ol_e_fscanf (file, e);
    ol_e_q (e, (*pSet).q[i]);
  }

  ol_e_free (e);

  return test;
}

int
ol_set_fscanf_sample (FILE * file, double factor, struct OL_SET *pSet, int* pf)
{
  /* needed for -O3 opti */
  struct OL_SET Setb = ol_set_alloc (1);

  ol_set_fscanf (file, &Setb, pf);
  ol_set_sample (Setb, factor, pSet);
  ol_set_free (Setb);

  return 1;
}

int
ol_set_fscanf_sample_nb (FILE * file, int nb, struct OL_SET *pSet, int* pf)
{
  /* needed for -O3 opti */
  struct OL_SET Setb = ol_set_alloc (1);

  ol_set_fscanf (file, &Setb, pf);
  ol_set_sample_nb (Setb, nb, pSet);
  ol_set_free (Setb);

  return 1;
}

int
ol_set_fscanf_sample_short (FILE * file, double factor, struct OL_SET *pSet)
{
  /* needed for -O3 opti */
  struct OL_SET Setb = ol_set_alloc (1);

  ol_set_fscanf_short (file, &Setb);
  ol_set_sample (Setb, factor, pSet);
  ol_set_free (Setb);

  return 1;
}

int
ol_set_fprintf (FILE * file, struct OL_SET Set, int f, char *format)
{
  unsigned int i;
  int test;
  double *e = ol_e_alloc ();
  char *fmt = ut_alloc_1d_char (4 * strlen (format) + 10);

  test = -1;

  if (f == 3)
  {
    test = fprintf (file, "%u\n", (unsigned int) Set.size);
    sprintf (fmt, "%s %s %s\n", format, format, format);
  }
  else if (f == 2)
    sprintf (fmt, "%s %s %s %s\n", format, format, format, format);
  else if (f == 1)
    sprintf (fmt, "%s %s %s\n", format, format, format);
  else
    abort ();

  if (f == 1)
    for (i = 0; i < Set.size; i++)
    {
      ol_q_e (Set.q[i], e);
      test = fprintf (file, fmt, e[0], e[1], e[2]);
    }
  else
    for (i = 0; i < Set.size; i++)
    {
      ol_q_e (Set.q[i], e);
      test = fprintf (file, fmt, e[0], e[1], e[2], Set.weight[i]);
    }

  ol_e_free (e);
  ut_free_1d_char (fmt);

  return test;
}

int
ol_set_fprintf_short (FILE * file, struct OL_SET Set, char *format)
{
  unsigned int i;
  int test = 0;
  double *e = ol_e_alloc ();

  for (i = 0; i < Set.size; i++)
  {
    ol_q_e (Set.q[i], e);
    test = ol_e_fprintf (file, e, format);
  }

  ol_e_free (e);

  return test;
}

void
ol_set_subset (struct OL_SET Set1, double factor, struct OL_SET *pSet2)
{
  unsigned int qty, id;
  double factorinv;

  if (factor < 1e-6)
    return;

  factorinv = 1 / factor;

  qty = (unsigned int) ((double) Set1.size * factor) + 2;

  (*pSet2).size = 0;
  (*pSet2).q = ut_alloc_2d (qty, 4);
  (*pSet2).weight = ut_alloc_1d (qty);

  id = (int) (factorinv * .5 - .5);
  while (id < Set1.size)
  {
    ol_q_memcpy (Set1.q[id], (*pSet2).q[(*pSet2).size]);
    (*pSet2).weight[(*pSet2).size] = Set1.weight[id];
    (*pSet2).size++;
    id = (int) (factorinv * ((*pSet2).size + 0.5) - 0.5);
  }

  return;
}

void
ol_set_orthotropic (struct OL_SET Set1, struct OL_SET *pSet2)
{
  unsigned int i, j, id;
  double **qsym = ut_alloc_2d (3, 4);

  (*pSet2) = ol_set_alloc (Set1.size * 4);

  id = 0;
  for (i = 0; i < Set1.size; i++)
  {
    ol_q_refsym (Set1.q[i], 1, qsym[0]);
    ol_q_refsym (Set1.q[i], 2, qsym[1]);
    ol_q_refsym (qsym[0], 2, qsym[2]);

    ol_q_memcpy (Set1.q[i], (*pSet2).q[id++]);
    for (j = 0; j < 3; j++)
      ol_q_memcpy (qsym[j], (*pSet2).q[id++]);
  }

  ut_free_2d (qsym, 3);

  return;
}
  
void
ol_set_memcpy (struct OL_SET Set1, struct OL_SET* pSet2)
{
  (*pSet2).size = Set1.size;
  ut_array_2d_memcpy ((*pSet2).q, Set1.size, 4, Set1.q);
  ut_array_1d_memcpy ((*pSet2).weight, Set1.size, Set1.weight);

  return;
}
