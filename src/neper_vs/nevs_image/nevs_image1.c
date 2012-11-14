/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_image.h"

void
nevs_image (char** argv, int* pi, struct PRINT* pPrint)
{
  int j;

  if (strcmp (argv[*pi], "-imagesize") == 0)
  {
    sscanf (argv[++(*pi)], "%d", &((*pPrint).imagewidth));
    sscanf (argv[++(*pi)], "%d", &((*pPrint).imageheight));
  }
  
  else if (strcmp (argv[*pi], "-imageantialias") == 0)
    sscanf (argv[++(*pi)], "%d", &((*pPrint).imageantialias));

  else if (strcmp (argv[*pi], "-imagebackground") == 0)
    for (j = 0; j < 3; j++)
      sscanf (argv[++(*pi)], "%lf", &((*pPrint).imagebackground[j]));
  
  else if (strcmp (argv[*pi], "-imageformat") == 0)
    sscanf (argv[++(*pi)], "%s", (*pPrint).format);

  else
  {
    ut_print_message (2, 0, "Unknown command %s\n", argv[(*pi)]);
    abort ();
  }

  return;
}
