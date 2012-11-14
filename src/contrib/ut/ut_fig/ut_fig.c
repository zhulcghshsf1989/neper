/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "../ut_fig.h"
#include"../ut.h"

void
ut_fig_header (FILE * file)
{
  fprintf (file, "#FIG 3.2\n");
  fprintf (file, "Landscape\n");
  fprintf (file, "Center\n");
  fprintf (file, "Metric\n");
  fprintf (file, "A4\n");
  fprintf (file, "100.00\n");
  fprintf (file, "Single\n");
  fprintf (file, "-2\n");
  fprintf (file, "1200 2\n");

  return;
}

void
ut_fig_polyline_box (FILE * file, int x1, int x2, int y1, int y2, int width,
		     int colour)
{
  fprintf (file, "2 2 0 %d %d 7 50 -1 -1 0.000 0 0 -1 0 0 5\n", width,
	   colour);
  fprintf (file, "\t\t%d %d %d %d %d %d %d %d %d %d\n\n", x1, y1, x2, y1, x2,
	   y2, x1, y2, x1, y1);
  return;
}

void
ut_fig_polyline_fillbox (FILE * file, int x1, int x2, int y1, int y2,
			 int width, int colour)
{
  fprintf (file, "2 2 0 %d %d %d 50 -1 20 0.000 0 0 -1 0 0 5\n", width,
	   colour, colour);
  fprintf (file, "\t\t%d %d %d %d %d %d %d %d %d %d\n\n",
	   x1, y1, x2, y1, x2, y2, x1, y2, x1, y1);
  return;
}

void
ut_fig_text (FILE * file, int x, int y, int jus, int font, int colour,
	     char *text)
{
  int length = (int) (strlen (text) * 125 * (font / 14));

  fprintf (file, "4 %d %d 50 -1 0 %d 0.0000 4 165 %d %d %d %s\\001\n",
	   jus, colour, font, length, x, y, text);

  return;
}
