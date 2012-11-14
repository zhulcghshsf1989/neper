/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "OperUtils.h"

int
oneDIntMin (int *array, int beg, int end)
{
  int min;
  int i;

  min = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] < min)
      min = array[i];

  return min;
}

int
oneDIntMax (int *array, int beg, int end)
{
  int max;
  int i;

  max = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] > max)
      max = array[i];

  return max;
}

int
oneDIntSum (int *array, int beg, int end)
{
  int sum;
  int i;

  sum = 0;
  for (i = beg; i <= end; i++)
    sum += array[i];

  return sum;
}

int
oneDIntNbOfThisVal (int *array, int beg, int end, int val)
{
  int qty = 0;
  int i;

  for (i = beg; i <= end; i++)
    if (array[i] == val)
      qty++;

  return qty;
}

float
oneDFloatMin (float *array, int beg, int end)
{
  float min;
  int i;

  min = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] < min)
      min = array[i];

  return min;
}

float
oneDFloatMax (float *array, int beg, int end)
{
  float max;
  int i;

  max = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] > max)
      max = array[i];

  return max;
}

double
oneDDoubleMin (double *array, int beg, int end)
{
  double min;
  int i;

  min = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] < min)
      min = array[i];

  return min;
}

double
oneDDoubleMax (double *array, int beg, int end)
{
  double max;
  int i;

  max = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] > max)
      max = array[i];

  return max;
}

double
oneDDoubleSum (double *array, int beg, int end)
{
  double sum;
  int i;

  sum = 0;
  for (i = beg; i <= end; i++)
    sum += array[i];

  return sum;
}

int
oneDDoubleMaxPos (double *array, int beg, int end)
{
  double max;
  int pos;
  int i;

  pos = beg;
  max = array[beg];
  for (i = beg + 1; i <= end; i++)
    if (array[i] > max)
    {
      max = array[i];
      pos = i;
    }

  return pos;
}


int
oneDDoubleAMaxPos (double *array, int beg, int end)
{
  double max;
  int pos;
  int i;

  pos = beg;
  max = fabs (array[beg]);
  for (i = beg + 1; i <= end; i++)
    if (fabs (array[i]) > max)
    {
      max = fabs (array[i]);
      pos = i;
    }

  return pos;
}

void
oneDDoubleScale (double *array, int beg, int end, double factor)
{
  int i;

  for (i = beg; i <= end; i++)
    array[i] *= factor;

  return;
}

void
oneDIntRevElts (int *array, int n1, int n2)
{
  int tmp;

  tmp = array[n1];
  array[n1] = array[n2];
  array[n2] = tmp;

  return;
}

void
oneDDoubleRevElts (double *array, int n1, int n2)
{
  double tmp;

  tmp = array[n1];
  array[n1] = array[n2];
  array[n2] = tmp;

  return;
}

/* oneDRotPos supplies the position situated at near position of pos. */
int
oneDRotPos (int beg, int end, int pos, int near)
{
  pos += near;

  while (pos > end)
    pos -= (end - beg + 1);

  while (pos < beg)
    pos += (end - beg + 1);

  return pos;
}

void
oneDIntRotElts (int *array, int beg, int end, int num)
{
  int i;
  int *copy;

  copy = (int *) ut_alloc_1d_int (end + 1);

  for (i = beg; i <= end; i++)
    copy[i] = array[i];

  for (i = beg; i <= end; i++)
    array[i] = copy[oneDRotPos (beg, end, i, num)];

  free (copy);

  return;
}

/* oneDIntEltPos searches value from beg to end in array. It records
 * its position in array and add the value near. If the position such
 * obtained is out of the array (lower than beg or higher than end),
 * it is modified: if position becomes higher than end, its value goes
 * again from beg: end+1<--beg. That is the same for position<beg.
 * 3 4 5 6 7 8 9 10 11 12 13 14 15 (beg=3, end=15)
 * 3--15 19 --> 19-(15-3+1)
 * 3--15 30 --> 30-(15-3+1)-(15-3+1)
 * 3--15  1 --> 1 +(15-3+1)
 * 3--15 -1 --> -1+(15-3+1)
 * 3--15 -10--> -10+(15-3+1)
 */
int
oneDIntEltPos (int *array, int beg, int end, int value, int near)
{
  int i;
  int pos = -1;

  for (i = beg; i <= end; i++)
    if (array[i] == value)
    {
      pos = i;
      break;
    }

  if (pos != -1 && near == 0)
    return pos;

  if (pos != -1)
    return oneDRotPos (beg, end, pos, near);
  else
    return -1;
}

/* The oneDIntSort subroutine sorts the array from element beg */
/* to element end, using the bubble sorting algorithm. */
void
oneDIntSort (int *array, int beg, int end)
{
  int i, j;

  for (i = 1; i <= end - beg; i++)
    for (j = end; j >= beg + i; j--)
      if (array[j - 1] > array[j])
	oneDIntRevElts (array, j - 1, j);

  return;
}

/* The oneDIntCompress subroutine compress the array from element beg */
/* to element end. A previous oneDIntSort step is required. */
int
oneDIntCompress (int *array, int beg, int end)
{
  int i;
  int end2;
  int *array2;

  array2 = ut_alloc_1d_int (end + 1);

  end2 = beg - 1;
  for (i = beg; i <= end - 1; i++)
    if (array[i] != array[i + 1])
    {
      end2++;
      array2[end2] = i;
    }
  end2++;
  array2[end2] = end;

  for (i = beg; i <= end2; i++)
    array[i] = array[array2[i]];
  for (i = end2 + 1; i <= end; i++)
    array[i] = 0;

  oneDIntFree (array2);

  return end2;
}

int
oneDIntSortNCompress (int *array, int beg, int end)
{
  int end2;

  oneDIntSort (array, beg, end);
  end2 = oneDIntCompress (array, beg, end);

  return end2;
}

/* oneDIntDeleteNCompress deletes element value from array and compresses */
/* the resulting array, loop times. */
int
oneDIntDeleteNCompress (int *array, int beg, int end, int value, int loop)
{
  int l;
  int i;
  int qty = 0;
  int pos;

  for (l = 1; l <= loop; l++)
  {
    pos = oneDIntEltPos (array, beg, end, value, 0);
    if (pos != -1)
    {
      for (i = pos; i <= end - 1; i++)
	array[i] = array[i + 1];
      qty++;
    }
    else
      break;
  }

  return qty;
}

int *
oneDIntCat (int *a, int bega, int enda, int *b, int begb, int endb,
	    int begres)
{
  int i;
  int elt;
  int *res;

  res = ut_alloc_1d_int (enda - bega + 1 + endb - begb + 1 + begres);

  elt = begres;
  for (i = bega; i <= enda; i++)
  {
    res[elt] = a[i];
    elt++;
  }
  for (i = begb; i <= endb; i++)
  {
    res[elt] = b[i];
    elt++;
  }

  return res;
}


void
oneDIntCpy (int *a, int bega, int enda, int *b, int begb, int alloc)
{
  int i;
  int qty;

  qty = enda - bega + 1;

  if (alloc == 1)		/* test this! */
    b = ut_alloc_1d_int (begb + qty);

  for (i = 0; i <= qty - 1; i++)
    b[begb + i] = a[bega + i];

  return;
}

int
oneDIntFindNReplace (int *array, int beg, int end, int old, int nnew,
		     int loop)
{
  int i;
  int pos;
  int qty = 0;

  for (i = 1; i <= loop; i++)
  {
    pos = oneDIntEltPos (array, beg, end, old, 0);
    if (pos != -1)
    {
      qty++;
      array[pos] = nnew;
    }
    else
      break;
  }

  return qty;
}

int
oneDIntCommonElts (int *array1, int beg1, int qty1,
		   int *array2, int beg2, int qty2, int **pres, int begres)
{
  int i;
  int tmpqty;
  int *tmp = ut_alloc_1d_int (qty1);

  tmpqty = 0;
  for (i = beg1; i <= qty1 + beg1 - 1; i++)	/* for every array1 elt, checking if it belongs */
  {				/* to array2. */

    /*printf("array1[%d]=%d\n",i,array1[i]); */

    if (oneDIntEltPos (array2, beg2, qty2 + beg2 - 1, array1[i], 0) != -1)
    {
      tmp[tmpqty] = array1[i];
      tmpqty++;
      /*printf("recorded\n"); */
    }
  }

  /*
     printf("tmpqty=%d\n",tmpqty);
     for(i=0;i<=tmpqty-1;i++)
     printf("tmp%d: %d\n",i,tmp[i]);
   */

  (*pres) = ut_alloc_1d_int (begres + tmpqty);
  oneDIntCpy (tmp, 0, tmpqty - 1, (*pres), begres, 0);

  oneDIntFree (tmp);

  return tmpqty;
}

void
oneDDoubleCpy (double *a, int bega, int enda, double *b, int begb, int alloc)
{
  int i;
  int qty;

  qty = enda - bega + 1;

  if (alloc == 1)		/* test this! */
    b = oneDDoubleDAlloc (begb + qty);

  for (i = 0; i <= qty - 1; i++)
    b[begb + i] = a[bega + i];

  return;
}
