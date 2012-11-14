/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_print_header.h"

void
nevs_print_header (FILE* file, struct PRINT Print)
{
  fprintf (file, "background {color rgb<%f,%f,%f>}\n",
		  Print.imagebackground[0],
		  Print.imagebackground[1],
		  Print.imagebackground[2]);

  fprintf (file, "camera {\n");
  fprintf (file, "%s\n", Print.cameraprojection);

  fprintf (file, "right x*image_width/image_height\n");
  fprintf (file, "location <%f, %f, %f>\n",
                  Print.cameracoo[0],
                  Print.cameracoo[2],
                  Print.cameracoo[1]);

  fprintf (file, "look_at <%f, %f, %f>\n",
                 Print.cameralookat[0],
                 Print.cameralookat[2],
                 Print.cameralookat[1]);

  fprintf (file, "angle %f\n", Print.cameraangle);

  fprintf (file, "}\n");

  if (Print.showshadow == 1)
    fprintf (file, "light_source { <%f, %f, %f> rgb<1, 1, 1>  shadowless }\n\n",
      Print.cameracoo[0], Print.cameracoo[2], Print.cameracoo[1]);

  fprintf (file, "union {\n\n");

  return;
}
