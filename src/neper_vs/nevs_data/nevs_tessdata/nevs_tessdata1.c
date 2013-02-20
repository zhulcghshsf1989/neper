/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_tessdata.h"

void
nevs_tessdata_init (struct TESS Tess, struct TESSDATA* pTessData)
{
  int id, qty;
  double vol, edgerad, verrad;
  
  (*pTessData).col[0] = ut_alloc_2d_int ((*pTessData).polyqty + 1, 3);
  (*pTessData).col[1] = ut_alloc_2d_int ((*pTessData).faceqty + 1, 3);
  (*pTessData).col[2] = ut_alloc_2d_int ((*pTessData).edgeqty + 1, 3);
  (*pTessData).col[3] = ut_alloc_2d_int ((*pTessData).verqty  + 1, 3);
  (*pTessData).rad[2] = ut_alloc_1d     ((*pTessData).edgeqty + 1);
  (*pTessData).rad[3] = ut_alloc_1d     ((*pTessData).verqty  + 1);

  neut_tess_volume (Tess, &vol);
  vol /= Tess.PolyQty;

  for (id = 0; id <= 3; id++)
  {
    neut_tessdata_id_qty (*pTessData, id, &qty);

    if (id == 0)
      ut_array_2d_int_set ((*pTessData).col[id] + 1, qty, 3, 255.);
    else if (id == 1)
      ut_array_2d_int_set ((*pTessData).col[id] + 1, qty, 3,   0.);
    else if (id == 2)
    {
      edgerad = pow (vol, 0.3333333333333) * 0.012;

      ut_array_2d_int_set ((*pTessData).col[id] + 1, qty, 3,   0.);
      ut_array_1d_set     ((*pTessData).rad[id] + 1, qty, edgerad);
    }
    else if (id == 3)
    {
      verrad = pow (vol, 0.3333333333333) * 0.012;

      ut_array_2d_int_set ((*pTessData).col[id] + 1, qty, 3,  0.);
      ut_array_1d_set     ((*pTessData).rad[id] + 1, qty, verrad);
    }
    else
      continue;

    if ((*pTessData).coldata[id] != NULL)
    {
      if (! strcmp ((*pTessData).coldatatype[id], "col"))
	nevs_data_col_colour ((*pTessData).coldata[id], qty,
			      (*pTessData).col[id]);
      else if (! strncmp ((*pTessData).coldatatype[id], "ori", 3))
	nevs_data_ori_colour ((*pTessData).coldata[id], qty,
			      "R", (*pTessData).col[id]);
      else if (! strcmp ((*pTessData).coldatatype[id], "scal"))
      {
	nevs_data_scal_colour ((*pTessData).coldata[id], qty, NULL, NULL, 
			       "blue,cyan,yellow,red", (*pTessData).col[id]);
	
	double val;
	nevs_data_scal_min ((*pTessData).coldata[id], qty, (*pTessData).scalemin[id], &val);
	(*pTessData).scalemin[id] = ut_realloc_1d_char ((*pTessData).scalemin[id], 100);
	sprintf ((*pTessData).scalemin[id], "%.15f", val);
	nevs_data_scal_max ((*pTessData).coldata[id], qty, (*pTessData).scalemax[id], &val);
	(*pTessData).scalemax[id] = ut_realloc_1d_char ((*pTessData).scalemax[id], 100);
	sprintf ((*pTessData).scalemax[id], "%.15f", val);
      }
      else
	ut_error_reportbug ();
    }
    
    if ((*pTessData).raddata[id] != NULL)
      nevs_data_rad_radius ((*pTessData).raddata[id], qty,
			    (*pTessData).rad[id]);
  }

  return;
}

void
nevs_tessdata_mtessdata (struct MTESS MTess, struct TESS* Tess, int levelmax,
			 struct TESSDATA TessData, struct TESSDATA** pMTessData)
{
  int i, j, tessid, level;
  struct TESSDATA* pTessData = NULL; // shortcut

  (*pMTessData) = calloc (MTess.TessQty + 1, sizeof (TESSDATA));
  pTessData = &((*pMTessData)[1]);

  neut_tessdata_set_default (pTessData);

  (*pTessData).verqty  = TessData.verqty;
  (*pTessData).edgeqty = TessData.edgeqty;
  (*pTessData).faceqty = TessData.faceqty;
  (*pTessData).polyqty = TessData.polyqty;

  (*pTessData).col[0] = ut_alloc_2d_int ((*pTessData).polyqty + 1, 3);
  ut_array_2d_int_memcpy ((*pTessData).col[0] + 1, (*pTessData).polyqty,
    3, TessData.col[0] + 1);

  (*pTessData).col[1] = ut_alloc_2d_int ((*pTessData).faceqty + 1, 3);
  ut_array_2d_int_memcpy ((*pTessData).col[1] + 1, (*pTessData).faceqty,
    3, TessData.col[1] + 1);
  (*pTessData).col[2] = ut_alloc_2d_int ((*pTessData).edgeqty + 1, 3);
  ut_array_2d_int_memcpy ((*pTessData).col[2] + 1, (*pTessData).edgeqty,
    3, TessData.col[2] + 1);
  (*pTessData).col[3] = ut_alloc_2d_int ((*pTessData).verqty  + 1, 3);
  ut_array_2d_int_memcpy ((*pTessData).col[3] + 1, (*pTessData).verqty,
    3, TessData.col[3] + 1);
  (*pTessData).rad[2] = ut_alloc_1d     ((*pTessData).edgeqty + 1);
  ut_array_1d_memcpy ((*pTessData).rad[2] + 1, (*pTessData).edgeqty,
    TessData.rad[2] + 1);
  (*pTessData).rad[3] = ut_alloc_1d     ((*pTessData).verqty  + 1);
  ut_array_1d_memcpy ((*pTessData).rad[3] + 1, (*pTessData).verqty,
    TessData.rad[3] + 1);

  if (levelmax == -1)
    levelmax = MTess.LevelQty;

  for (level = 2; level <= levelmax; level++)
  {
    for (i = 1; i <= MTess.LevelTessQty[level]; i++)
    {
      tessid = MTess.LevelTess[level][i];
      pTessData = &((*pMTessData)[tessid]);

      neut_tessdata_set_default (pTessData);

      (*pTessData).verqty  = Tess[tessid].VerQty;
      (*pTessData).edgeqty = Tess[tessid].EdgeQty;
      (*pTessData).faceqty = Tess[tessid].FaceQty;
      (*pTessData).polyqty = Tess[tessid].PolyQty;

      (*pTessData).col[0] = ut_alloc_2d_int ((*pTessData).polyqty + 1, 3);
      (*pTessData).col[1] = ut_alloc_2d_int ((*pTessData).faceqty + 1, 3);
      (*pTessData).col[2] = ut_alloc_2d_int ((*pTessData).edgeqty + 1, 3);
      (*pTessData).col[3] = ut_alloc_2d_int ((*pTessData).verqty  + 1, 3);
      (*pTessData).rad[2] = ut_alloc_1d     ((*pTessData).edgeqty + 1);
      (*pTessData).rad[3] = ut_alloc_1d     ((*pTessData).verqty  + 1);

      // Polys
      int* dom = MTess.TessDom[tessid];

      for (j = 1; j <= (*pTessData).polyqty; j++)
	ut_array_1d_int_memcpy ((*pTessData).col[0][j], 3,
	    (*pMTessData)[dom[0]].col[0][dom[1]]);

      // Edges
      for (j = 1; j <= (*pTessData).edgeqty; j++)
	(*pTessData).rad[2][j] = (*pMTessData)[dom[0]].rad[2][1];
    }
  }

  return;
}

void
nevs_tessdata_fscanf (struct TESS Tess, char* entity, char* type, char* argument,
                     struct TESSDATA* pTessData)
{
  int id;
  int* qty = ut_alloc_1d_int (4);

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, type);

  (*pTessData).verqty  = Tess.VerQty;
  (*pTessData).edgeqty = Tess.EdgeQty;
  (*pTessData).faceqty = Tess.FaceQty;
  (*pTessData).polyqty = Tess.PolyQty;

  if (strcmp (entity, "poly") == 0)
  {
    id = 0;
    qty[id] = (*pTessData).polyqty;
  }
  else if (strcmp (entity, "face") == 0)
  {
    id = 1;
    qty[id] = (*pTessData).faceqty;
  }
  else if (strcmp (entity, "edge") == 0)
  {
    id = 2;
    qty[id] = (*pTessData).edgeqty;
  }
  else if (strcmp (entity, "ver") == 0)
  {
    id = 3;
    qty[id] = (*pTessData).verqty;
  }
  else
    abort ();

  char** args = NULL;
  int    argqty;
  ut_string_separate (argument, '=', &args, &argqty);

  if (! strcmp (type, "col"))
  {
    (*pTessData).coldatatype[id] = ut_alloc_1d_char (100);
    char* value = ut_alloc_1d_char (1000);

    nevs_data_colarg_args (argument, (*pTessData).coldatatype[id], value);
    
    if (! strcmp ((*pTessData).coldatatype[id], "col"))
    {
      (*pTessData).coldata[id] = ut_alloc_2d (qty[id] + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pTessData).coldata[id] + 1,
				   qty[id], 3, "colour,size");
    }
    else if (! strncmp ((*pTessData).coldatatype[id], "ori", 3))
    {
      (*pTessData).coldata[id] = ut_alloc_2d (qty[id] + 1, 3);
      int i;
      double*  tmpd  = ut_alloc_1d     (4);
      int*     tmpi  = ut_alloc_1d_int (6);
      double** tmpdd = ut_alloc_2d     (3, 3);
      FILE* file = ut_file_open (value, "r");

      int qty0;
      if (! strcmp ((*pTessData).coldatatype[id] + 3, "e" )
       || ! strcmp ((*pTessData).coldatatype[id] + 3, "ek")
       || ! strcmp ((*pTessData).coldatatype[id] + 3, "er")
       || ! strcmp ((*pTessData).coldatatype[id] + 3, "R" ))
	qty0 = 3;
      else if (! strcmp ((*pTessData).coldatatype[id] + 3, "q"))
	qty0 = 4;
      else if (! strcmp ((*pTessData).coldatatype[id] + 3, "m"))
	qty0 = 6;
      else if (! strcmp ((*pTessData).coldatatype[id] + 3, "g"))
	qty0 = 9;
      else
      {
	printf ("(*pTessData).coldatatype[id] = %s\n", (*pTessData).coldatatype[id]);
	ut_error_reportbug ();
	abort ();
      }

      ut_file_nbwords_testwmessage (value, qty[id] * qty0);

      for (i = 1; i <= qty[id]; i++)
	if (! strcmp ((*pTessData).coldatatype[id] + 3, "e"))
	  ol_e_fscanf (file, (*pTessData).coldata[id][i]);
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "ek"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_ek_e (tmpd, (*pTessData).coldata[id][i]);
	}
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "er"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_er_e (tmpd, (*pTessData).coldata[id][i]);
	}
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "q"))
	{
	  ol_q_fscanf (file, tmpd);
	  ol_q_e (tmpd, (*pTessData).coldata[id][i]);
	}
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "R"))
	{
	  ol_R_fscanf (file, tmpd);
	  ol_R_e (tmpd, (*pTessData).coldata[id][i]);
	}
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "m"))
	{
	  ol_m_fscanf (file, tmpi);
	  ol_m_e (tmpi, (*pTessData).coldata[id][i]);
	}
	else if (! strcmp ((*pTessData).coldatatype[id] + 3, "g"))
	{
	  ol_g_fscanf (file, tmpdd);
	  ol_g_e (tmpdd, (*pTessData).coldata[id][i]);
	}
	else
	  ut_error_reportbug ();
    }
    else if (! strcmp ((*pTessData).coldatatype[id], "scal"))
    {
      (*pTessData).coldata[id] = ut_alloc_2d (qty[id] + 1, 1);
	ut_array_2d_fscanfn_wcard (value, (*pTessData).coldata[id] + 1,
				   qty[id], 1, "numeral,size");
    }
    else
      abort ();

    ut_free_1d_char (value);
  }
  else if (! strcmp (type, "rad"))
  {
    (*pTessData).raddatatype[id] = ut_alloc_1d_char (100);

    if (argqty == 1)
      strcpy ((*pTessData).raddatatype[id], type);
    else
      strcpy ((*pTessData).raddatatype[id], args[0]);

    if (! strcmp ((*pTessData).raddatatype[id], "rad"))
    {
      (*pTessData).raddata[id] = ut_alloc_2d (qty[id] + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pTessData).raddata[id] + 1,
				 qty[id], 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (! strcmp (type, "colscheme"))
  {
    (*pTessData).colscheme[id] = ut_alloc_1d_char (100);
    strcpy ((*pTessData).colscheme[id], argument);
  }
  else if (! strcmp (type, "scalemin"))
  {
    (*pTessData).scalemin[id] = ut_alloc_1d_char (100);
    strcpy ((*pTessData).scalemin[id], argument);
  }
  else if (! strcmp (type, "scalemax"))
  {
    (*pTessData).scalemax[id] = ut_alloc_1d_char (100);
    strcpy ((*pTessData).scalemax[id], argument);
  }
  else if (! strcmp (type, "scaleticks"))
  {
    (*pTessData).scaleticks[id] = ut_alloc_1d_char (100);
    strcpy ((*pTessData).scaleticks[id], argument);
  }
  else
    ut_error_reportbug ();

  ut_free_2d_char (args, argqty);
  ut_free_1d_int (qty);
  
  return;
}
