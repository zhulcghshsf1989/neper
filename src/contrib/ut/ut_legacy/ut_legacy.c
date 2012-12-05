/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<math.h>
#include "ut_legacy.h"
#include "ut.h"

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
    return ut_num_rotpos (beg, end, pos, near);
  else
    return -1;
}

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

  ut_free_1d_int (array2);

  return end2;
}

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
  ut_array_1d_int_memcpy ( (*pres) + begres, tmpqty, tmp);

  ut_free_1d_int (tmp);

  return tmpqty;
}
