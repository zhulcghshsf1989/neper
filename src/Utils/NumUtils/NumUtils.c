/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "NumUtils.h"

/* Number tools ------------------------------------------------------------- */
int
IntMin (int num1, int num2)
{
  if (num1 < num2)
    return num1;
  else
    return num2;
}

int
IntMax (int num1, int num2)
{
  if (num1 > num2)
    return num1;
  else
    return num2;
}

double
DoubleMin (double num1, double num2)
{
  if (num1 < num2)
    return num1;
  else
    return num2;
}

double
DoubleMax (double num1, double num2)
{
  if (num1 > num2)
    return num1;
  else
    return num2;
}

/* D2rI round the d double number to the nearer integer. */
int
D2rI (double d)
{
  if (d - floor (d) < 0.49999999999999)
    return (int) floor (d);
  else
    return (int) ceil (d);
}

int
IntTenLen (double num)
{
  int i;

  for (i = 0; i <= 8; i++)
    if ((double) num >= pow (10, i) && (double) num < pow (10, i + 1))
      return i;

  return -1;
}

/* NnId2Rand returns a 8-numbers long values if id>=0, a random
 * (and unreproductible) value.
 */
int
NnId2Rand (int N, int id)
{
  time_t t;
  int Rand;

  /* Rand: long integer number calculated from (N,id):
   * Rand is 8 numbers long ; N is put at the left side, id at the
   * right side. If N and id numbers cross, they are added.
   * N=123  and id=12     --> Rand=12300012
   * N=3000 and id=65433  --> Rand=30065433
   * N=3333 and id=88888  --> Rand=33418888
   */

  if (id >= 0)
    Rand = N * pow (10, 7 - IntTenLen (N)) + id;
  else
  {
    time (&t);
    Rand = t;
  }

  return Rand;
}

int
DoubleEq (double n1, double n2, double eps)
{
  if (fabs (n2 - n1) < eps)
    return 1;
  else
    return 0;
}
