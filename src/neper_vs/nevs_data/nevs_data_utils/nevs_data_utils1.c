/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_data_utils.h"

int
nevs_data_string_entity_type (char* string, char* entity, char* type)
{
  char* stringcpy = ut_alloc_1d_char (strlen (string) + 1);

  // skipping -data if present
  if (strncmp (string, "-data", 5) == 0)
    strcpy (stringcpy, string + 5);
  else
    strcpy (stringcpy, string);

  // setting entity: must be one of:
  // for the mesh: node, elt, elset, elt1d, eltedge
  // for the tess: edge, poly
  if      (strncmp (stringcpy, "node2elt", 8) == 0)
    strcpy (entity, "node2elt");
  else if (strncmp (stringcpy, "node"   , 4) == 0)
    strcpy (entity, "node");
  else if (strncmp (stringcpy, "elt3dnode", 9) == 0)
    strcpy (entity, "elt3dnode");
  else if (strncmp (stringcpy, "elt3dedge", 9) == 0)
    strcpy (entity, "elt3dedge");
  else if (strncmp (stringcpy, "eltedge", 7) == 0)
    strcpy (entity, "eltedge");
  else if (strncmp (stringcpy, "elt3d"  , 5) == 0)
    strcpy (entity, "elt3d");
  else if (strncmp (stringcpy, "elt2d"  , 5) == 0)
    strcpy (entity, "elt2d");
  else if (strncmp (stringcpy, "elt1d"  , 5) == 0)
    strcpy (entity, "elt1d");
  else if (strncmp (stringcpy, "elt0d"  , 5) == 0)
    strcpy (entity, "elt0d");
  else if (strncmp (stringcpy, "elt"    , 3) == 0)
    strcpy (entity, "elt");
  else if (strncmp (stringcpy, "elsetedge", 9) == 0)
    strcpy (entity, "elset3dedge");
  else if (strncmp (stringcpy, "elset3dedge", 11) == 0)
    strcpy (entity, "elset3dedge");
  else if (strncmp (stringcpy, "elset3d", 7) == 0)
    strcpy (entity, "elset3d");
  else if (strncmp (stringcpy, "elset2d", 7) == 0)
    strcpy (entity, "elset2d");
  else if (strncmp (stringcpy, "elset1d", 7) == 0)
    strcpy (entity, "elset1d");
  else if (strncmp (stringcpy, "elset0d", 7) == 0)
    strcpy (entity, "elset0d");
  else if (strncmp (stringcpy, "elset"  , 5) == 0)
    strcpy (entity, "elset");
  else if (strncmp (stringcpy, "ver"    , 3) == 0)
    strcpy (entity, "ver");
  else if (strncmp (stringcpy, "edge"   , 4) == 0)
    strcpy (entity, "edge");
  else if (strncmp (stringcpy, "face"   , 4) == 0)
    strcpy (entity, "face");
  else if (strncmp (stringcpy, "poly"   , 4) == 0)
    strcpy (entity, "poly");
  else
  {
    ut_print_message (2, 0, "option -data*: entity not supported");
    abort ();
  }
  
  // setting the type of data
  strcpy (type, stringcpy + strlen (entity)); 

  // fixing stuff
  if (strncmp (type, "colourscheme", 12) == 0
   || strncmp (type, "colorscheme" , 11) == 0)
    sprintf (type, "colscheme");
  else if (! strcmp (entity, "elset"))
    sprintf (entity, "elset3d");
  else if (! strcmp (entity, "elt"))
    sprintf (entity, "elt3d");
  else if (! strcmp (entity, "eltedge"))
    sprintf (entity, "elt3dedge");

  ut_free_1d_char (stringcpy);

  return 0;
}

int
nevs_data_type_size (char* type, int* psize)
{
  int status = 0;
  if (strcmp (type, "scal") == 0
   || strcmp (type, "int" ) == 0
   || strcmp (type, "rad" ) == 0
   || strcmp (type, "edgerad" ) == 0
   || strcmp (type, "real") == 0)
    (*psize) = 1;
  else if (strcmp (type, "ori") == 0
   || strcmp (type, "edgecol") == 0 
   || strcmp (type, "col") == 0 
   || strcmp (type, "coo") == 0)
    (*psize) = 3;
  else if (strncmp (type, "int", 3) == 0
   || strncmp (type, "real", 4) == 0)
    sscanf (type + 4, "%d", psize);
  else
    status = -1;

  return status;
}

void
nevs_data_col_colour (double** data, int s1, int** col)
{
  int i, j;

  for (i = 1; i <= s1; i++)
    for (j = 0; j < 3; j++)
      col[i][j] = ut_num_d2ri (data[i][j]);

  return;
}

void
nevs_data_rad_radius (double** data, int s1, double* rad)
{
  int i;

  for (i = 1; i <= s1; i++)
    rad[i] = data[i][0];

  return;
}

void
nevs_data_ori_colour (double** data, int s1, char* scheme, int** col)
{
  int i, j;
  double* R = ol_R_alloc ();

  if (scheme == NULL || ! strcmp (scheme, "R"))
    for (i = 1; i <= s1; i++)
    {
      ol_e_R (data[i], R);
      ol_R_Rcrysym (R, "cubic", R);
      for (j = 0; j < 3; j++)
	col[i][j] = ut_num_d2ri (255 * (R[j] + (OL_S2 - 1))
					/ (2 * (OL_S2 - 1)));
    }
  else
    ut_error_reportbug ();

  ol_R_free (R);

  return;
}


void
nevs_data_scal_min (double** data, int s1, char* min, double *pval)
{
  if (min == NULL || strlen (min) == 0 || ! strcmp (min, "min"))
    (*pval) = ut_array_2d_col_min (data + 1, 0, s1);
  else
    sscanf (min, "%lf", pval);

  return;
}

void
nevs_data_scal_max (double** data, int s1, char* max, double *pval)
{
  if (max == NULL || strlen (max) == 0 || ! strcmp (max, "max"))
    (*pval) = ut_array_2d_col_max (data + 1, 0, s1);
  else
    sscanf (max, "%lf", pval);

  return;
}

void
nevs_data_scal_colour (double** data, int s1, char* min, char* max, 
                       char* scheme, int** col)
{
  int i;
  double scalemin, scalemax;
  double datamin, datamax;

  datamin = ut_array_2d_col_min (data + 1, 0, s1);
  datamax = ut_array_2d_col_max (data + 1, 0, s1);

  if (min == NULL || strlen (min) == 0 || ! strcmp (min, "min"))
    scalemin = datamin;
  else
    sscanf (min, "%lf", &scalemin);

  if (max == NULL || strlen (max) == 0 || ! strcmp (max, "max"))
    scalemax = datamax;
  else
    sscanf (max, "%lf", &scalemax);

  if (scalemin > datamin)
    ut_print_message (1, 2, "Scale minimum (%f) larger  than data minimum (%f)\n", scalemin, datamin);

  if (scalemax < datamax)
    ut_print_message (1, 2, "Scale maximum (%f) smaller than data maximum (%f)\n", scalemax, datamax);
  
  if (scheme == NULL)
    for (i = 1; i <= s1; i++)
      ut_color_scheme_val_color ("blue,cyan,yellow,red",
	                         scalemin, scalemax, data[i][0], col[i]);
  else
    for (i = 1; i <= s1; i++)
      ut_color_scheme_val_color (scheme, scalemin, scalemax, data[i][0], col[i]);

  return;
}

void
nevs_data_coo_coo (double** ref, double** data, double fact, int s1, double** coo)
{
  int i, j;

  for (i = 1; i <= s1; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * (data[i][j] - ref[i][j]);

  return;
}

void
nevs_data_disp_coo (double** ref, double** data, double fact, int s1, double** coo)
{
  int i, j;

  for (i = 1; i <= s1; i++)
    for (j = 0; j < 3; j++)
      coo[i][j] = ref[i][j] + fact * data[i][j];

  return;
}

void
nevs_data_colarg_args (char* argument, char* type, char* value)
{
  char** args = NULL;
  int    argqty;

  ut_string_separate (argument, '=', &args, &argqty);

  if (argqty == 1)
  {
    sprintf (type, "col");
    sprintf (value, "%s", args[0]);
  }
  else
  {
    strcpy (type, args[0]);
    sprintf (value, "%s", args[1]);
  }

  ut_free_2d_char (args, argqty);

  if (! strcmp (type, "col"))
    return;
  else if (! strcmp (type, "e" )
   || ! strcmp (type, "ek")
   || ! strcmp (type, "er")
   || ! strcmp (type, "R" )
   || ! strcmp (type, "q" )
   || ! strcmp (type, "m" )
   || ! strcmp (type, "rtheta")
   || ! strcmp (type, "g"))
  {
    char* tmpc = ut_alloc_1d_char (100);
    tmpc = ut_string_paste ("ori", type);
    strcpy (type, tmpc);
    ut_free_1d_char (tmpc);
  }
  else if (! strncmp (type, "ori", 3))
     strcpy (type, "orie");
  // else
    // ut_error_reportbug ();

  return;
}

