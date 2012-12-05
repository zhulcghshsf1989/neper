/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/time.h>
#include"ut_time.h"

double
ut_time_subtract (struct timeval *x, struct timeval *y)
{
  double t;
  struct timeval result;

  /* Perform the carry for the later subtraction by updating y.
   * */
  if (x->tv_usec < y->tv_usec)
  {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
    y->tv_usec -= 1000000 * nsec;
    y->tv_sec += nsec;
  }
  if (x->tv_usec - y->tv_usec > 1000000)
  {
    int nsec = (y->tv_usec - x->tv_usec) / 1000000;
    y->tv_usec += 1000000 * nsec;
    y->tv_sec -= nsec;
  }

  /* Compute the time remaining to wait.
   *          *      tv_usec is certainly
   *          positive. */
  result.tv_sec = x->tv_sec - y->tv_sec;
  result.tv_usec = x->tv_usec - y->tv_usec;

  t = -((double) result.tv_sec + (double) result.tv_usec / 1000000);

  return t;
}
