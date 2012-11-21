/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_geodata.h"

void
nevs_geodata_init (struct GEO Geo, struct GEODATA* pGeoData)
{
  int id, qty;
  double vol, edgerad, verrad;
  
  (*pGeoData).col[0] = ut_alloc_2d_int ((*pGeoData).polyqty + 1, 3);
  (*pGeoData).col[1] = ut_alloc_2d_int ((*pGeoData).faceqty + 1, 3);
  (*pGeoData).col[2] = ut_alloc_2d_int ((*pGeoData).edgeqty + 1, 3);
  (*pGeoData).col[3] = ut_alloc_2d_int ((*pGeoData).verqty  + 1, 3);
  (*pGeoData).rad[2] = ut_alloc_1d     ((*pGeoData).edgeqty + 1);
  (*pGeoData).rad[3] = ut_alloc_1d     ((*pGeoData).verqty  + 1);

  neut_geo_volume (Geo, &vol);
  vol /= Geo.PolyQty;

  for (id = 0; id <= 3; id++)
  {
    neut_geodata_id_qty (*pGeoData, id, &qty);

    if (id == 0)
      ut_array_2d_int_set ((*pGeoData).col[id] + 1, qty, 3, 255.);
    else if (id == 1)
      ut_array_2d_int_set ((*pGeoData).col[id] + 1, qty, 3,   0.);
    else if (id == 2)
    {
      edgerad = pow (vol, 0.3333333333333) * 0.012;

      ut_array_2d_int_set ((*pGeoData).col[id] + 1, qty, 3,   0.);
      ut_array_1d_set     ((*pGeoData).rad[id] + 1, qty, edgerad);
    }
    else if (id == 3)
    {
      verrad = pow (vol, 0.3333333333333) * 0.012;

      ut_array_2d_int_set ((*pGeoData).col[id] + 1, qty, 3,  0.);
      ut_array_1d_set     ((*pGeoData).rad[id] + 1, qty, verrad);
    }
    else
      continue;

    if ((*pGeoData).coldata[id] != NULL)
    {
      if (! strcmp ((*pGeoData).coldatatype[id], "col"))
	nevs_data_col_colour ((*pGeoData).coldata[id], qty,
			      (*pGeoData).col[id]);
      else if (! strncmp ((*pGeoData).coldatatype[id], "ori", 3))
	nevs_data_ori_colour ((*pGeoData).coldata[id], qty,
			      "R", (*pGeoData).col[id]);
      else if (! strcmp ((*pGeoData).coldatatype[id], "scal"))
      {
	nevs_data_scal_colour ((*pGeoData).coldata[id], qty, NULL, NULL, 
			       "blue,cyan,yellow,red", (*pGeoData).col[id]);
	
	double val;
	nevs_data_scal_min ((*pGeoData).coldata[id], qty, (*pGeoData).scalemin[id], &val);
	(*pGeoData).scalemin[id] = ut_realloc_1d_char ((*pGeoData).scalemin[id], 100);
	sprintf ((*pGeoData).scalemin[id], "%.15f", val);
	nevs_data_scal_max ((*pGeoData).coldata[id], qty, (*pGeoData).scalemax[id], &val);
	(*pGeoData).scalemax[id] = ut_realloc_1d_char ((*pGeoData).scalemax[id], 100);
	sprintf ((*pGeoData).scalemax[id], "%.15f", val);
      }
      else
	ut_error_reportbug ();
    }
    
    if ((*pGeoData).raddata[id] != NULL)
      nevs_data_rad_radius ((*pGeoData).raddata[id], qty,
			    (*pGeoData).rad[id]);
  }

  return;
}

void
nevs_geodata_fscanf (struct GEO Geo, char* entity, char* type, char* argument,
                     struct GEODATA* pGeoData)
{
  int id;
  int* qty = ut_alloc_1d_int (4);

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, type);

  (*pGeoData).verqty  = Geo.VerQty;
  (*pGeoData).edgeqty = Geo.EdgeQty;
  (*pGeoData).faceqty = Geo.FaceQty;
  (*pGeoData).polyqty = Geo.PolyQty;

  if (strcmp (entity, "poly") == 0)
  {
    id = 0;
    qty[id] = (*pGeoData).polyqty;
  }
  else if (strcmp (entity, "face") == 0)
  {
    id = 1;
    qty[id] = (*pGeoData).faceqty;
  }
  else if (strcmp (entity, "edge") == 0)
  {
    id = 2;
    qty[id] = (*pGeoData).edgeqty;
  }
  else if (strcmp (entity, "ver") == 0)
  {
    id = 3;
    qty[id] = (*pGeoData).verqty;
  }
  else
    abort ();

  char** args = NULL;
  int    argqty;
  ut_string_separate (argument, '=', &args, &argqty);

  if (! strcmp (type, "col"))
  {
    (*pGeoData).coldatatype[id] = ut_alloc_1d_char (100);
    char* value = ut_alloc_1d_char (1000);

    nevs_data_colarg_args (argument, (*pGeoData).coldatatype[id], value);
    
    if (! strcmp ((*pGeoData).coldatatype[id], "col"))
    {
      (*pGeoData).coldata[id] = ut_alloc_2d (qty[id] + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pGeoData).coldata[id] + 1,
				   qty[id], 3, "colour,size");
    }
    else if (! strncmp ((*pGeoData).coldatatype[id], "ori", 3))
    {
      (*pGeoData).coldata[id] = ut_alloc_2d (qty[id] + 1, 3);
      int i;
      double*  tmpd  = ut_alloc_1d     (4);
      int*     tmpi  = ut_alloc_1d_int (6);
      double** tmpdd = ut_alloc_2d     (3, 3);
      FILE* file = ut_file_open (value, "r");

      int qty0;
      if (! strcmp ((*pGeoData).coldatatype[id] + 3, "e" )
       || ! strcmp ((*pGeoData).coldatatype[id] + 3, "ek")
       || ! strcmp ((*pGeoData).coldatatype[id] + 3, "er")
       || ! strcmp ((*pGeoData).coldatatype[id] + 3, "R" ))
	qty0 = 3;
      else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "q"))
	qty0 = 4;
      else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "m"))
	qty0 = 6;
      else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "g"))
	qty0 = 9;
      else
      {
	printf ("(*pGeoData).coldatatype[id] = %s\n", (*pGeoData).coldatatype[id]);
	ut_error_reportbug ();
	abort ();
      }

      ut_file_nbwords_testwmessage (value, qty[id] * qty0);

      for (i = 1; i <= qty[id]; i++)
	if (! strcmp ((*pGeoData).coldatatype[id] + 3, "e"))
	  ol_e_fscanf (file, (*pGeoData).coldata[id][i]);
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "ek"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_ek_e (tmpd, (*pGeoData).coldata[id][i]);
	}
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "er"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_er_e (tmpd, (*pGeoData).coldata[id][i]);
	}
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "q"))
	{
	  ol_q_fscanf (file, tmpd);
	  ol_q_e (tmpd, (*pGeoData).coldata[id][i]);
	}
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "R"))
	{
	  ol_R_fscanf (file, tmpd);
	  ol_R_e (tmpd, (*pGeoData).coldata[id][i]);
	}
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "m"))
	{
	  ol_m_fscanf (file, tmpi);
	  ol_m_e (tmpi, (*pGeoData).coldata[id][i]);
	}
	else if (! strcmp ((*pGeoData).coldatatype[id] + 3, "g"))
	{
	  ol_g_fscanf (file, tmpdd);
	  ol_g_e (tmpdd, (*pGeoData).coldata[id][i]);
	}
	else
	  ut_error_reportbug ();
    }
    else if (! strcmp ((*pGeoData).coldatatype[id], "scal"))
    {
      (*pGeoData).coldata[id] = ut_alloc_2d (qty[id] + 1, 1);
	ut_array_2d_fscanfn_wcard (value, (*pGeoData).coldata[id] + 1,
				   qty[id], 1, "numeral,size");
    }
    else
      abort ();

    ut_free_1d_char (value);
  }
  else if (! strcmp (type, "rad"))
  {
    (*pGeoData).raddatatype[id] = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pGeoData).raddatatype[id], type);
    else
      strcpy ((*pGeoData).raddatatype[id], args[0]);

    if (! strcmp ((*pGeoData).raddatatype[id], "rad"))
    {
      (*pGeoData).raddata[id] = ut_alloc_2d (qty[id] + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pGeoData).raddata[id] + 1,
				 qty[id], 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (! strcmp (type, "colscheme"))
  {
    (*pGeoData).colscheme[id] = ut_alloc_1d_char (100);
    strcpy ((*pGeoData).colscheme[id], argument);
  }
  else if (! strcmp (type, "scalemin"))
  {
    (*pGeoData).scalemin[id] = ut_alloc_1d_char (100);
    strcpy ((*pGeoData).scalemin[id], argument);
  }
  else if (! strcmp (type, "scalemax"))
  {
    (*pGeoData).scalemax[id] = ut_alloc_1d_char (100);
    strcpy ((*pGeoData).scalemax[id], argument);
  }
  else if (! strcmp (type, "scaleticks"))
  {
    (*pGeoData).scaleticks[id] = ut_alloc_1d_char (100);
    strcpy ((*pGeoData).scaleticks[id], argument);
  }
  else
    ut_error_reportbug ();

  ut_free_2d_char (args, argqty);
  ut_free_1d_int (qty);
  
  return;
}
