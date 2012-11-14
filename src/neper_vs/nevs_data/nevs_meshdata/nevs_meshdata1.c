/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_meshdata.h"

void
nevs_meshdata_init (struct NODES Nodes, struct MESH Mesh3D, struct MESHDATA* pMeshData)
{
  int id, qty;
  double vol;
  double elt0drad, elt1drad, elt3dedgerad, noderad;
  int idmax;

  neut_meshdata_idmax (&idmax);

  int elt3d_id, elt2d_id, elt1d_id, elt0d_id, node_id, node2elt_id, elt3dedge_id;
  neut_meshdata_entity_id ("elt3d", &elt3d_id);
  neut_meshdata_entity_id ("elt2d", &elt2d_id);
  neut_meshdata_entity_id ("elt1d", &elt1d_id);
  neut_meshdata_entity_id ("elt0d", &elt0d_id);
  neut_meshdata_entity_id ("node", &node_id);
  neut_meshdata_entity_id ("node2elt", &node2elt_id);
  neut_meshdata_entity_id ("elt3dedge", &elt3dedge_id);

  if ((*pMeshData).col[elt3d_id] == NULL)
    (*pMeshData).col[elt3d_id]     = ut_alloc_2d_int ((*pMeshData).elt3dqty + 1, 3);

  if ((*pMeshData).col[elt2d_id] == NULL)
    (*pMeshData).col[elt2d_id]     = ut_alloc_2d_int ((*pMeshData).elt2dqty + 1, 3);

  if ((*pMeshData).col[elt1d_id] == NULL)
    (*pMeshData).col[elt1d_id]     = ut_alloc_2d_int ((*pMeshData).elt1dqty + 1, 3);

  if ((*pMeshData).col[elt0d_id] == NULL)
    (*pMeshData).col[elt0d_id]     = ut_alloc_2d_int ((*pMeshData).elt0dqty + 1, 3);

  if ((*pMeshData).col[node_id] == NULL)
    (*pMeshData).col[node_id ]     = ut_alloc_2d_int ((*pMeshData).nodeqty  + 1, 3);

  if ((*pMeshData).col[node2elt_id] == NULL)
    (*pMeshData).col[node2elt_id ] = ut_alloc_2d_int ((*pMeshData).nodeqty  + 1, 3);

  if ((*pMeshData).col[elt3dedge_id] == NULL)
    (*pMeshData).col[elt3dedge_id] = ut_alloc_2d_int ((*pMeshData).elt3dqty + 1, 3);

  if ((*pMeshData).rad[elt1d_id] == NULL)
    (*pMeshData).rad[elt1d_id]     = ut_alloc_1d     ((*pMeshData).elt1dqty + 1);

  if ((*pMeshData).rad[elt0d_id] == NULL)
    (*pMeshData).rad[elt0d_id]     = ut_alloc_1d     ((*pMeshData).elt0dqty + 1);

  if ((*pMeshData).rad[node_id] == NULL)
    (*pMeshData).rad[node_id ]     = ut_alloc_1d     ((*pMeshData).nodeqty  + 1);

  if ((*pMeshData).rad[elt3dedge_id] == NULL)
    (*pMeshData).rad[elt3dedge_id] = ut_alloc_1d     ((*pMeshData).elt3dqty + 1);

  if ((*pMeshData).coo[node_id] == NULL)
    (*pMeshData).coo[node_id]      = ut_alloc_2d     ((*pMeshData).nodeqty + 1, 3);

  if (Mesh3D.Dimension == 3)
  {
    neut_mesh_volume (Nodes, Mesh3D, &vol, NULL, NULL);
    vol /= (*pMeshData).elset3dqty;
  }
  else
    vol = 1;

  elt0drad     = pow (vol, 0.3333333333333) * 0.012;
  elt1drad     = pow (vol, 0.3333333333333) * 0.012;
  noderad      = pow (vol, 0.3333333333333) * 0.012;
  elt3dedgerad = pow (vol, 0.3333333333333) * 0.004;

  for (id = 0; id <= idmax; id++)
  {
    neut_meshdata_id_qty (*pMeshData, id, &qty);

    if (id == elt3d_id)
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 255.);
    else if (id == elt2d_id)
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
    else if (id == elt1d_id)
    {
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
      ut_array_1d_set     ((*pMeshData).rad[id] + 1, qty, elt1drad);
    }
    else if (id == elt0d_id)
    {
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
      ut_array_1d_set     ((*pMeshData).rad[id] + 1, qty, elt0drad);
    }
    else if (id == node_id)
    {
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
      ut_array_1d_set     ((*pMeshData).rad[id] + 1, qty, noderad);

      ut_array_2d_memcpy  ((*pMeshData).coo[id] + 1, qty, 3, Nodes.NodeCoo + 1);
    }
    else if (id == node2elt_id)
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
    else if (id == elt3dedge_id) 
    {
      ut_array_2d_int_set ((*pMeshData).col[id] + 1, qty, 3, 0.);
      ut_array_1d_set     ((*pMeshData).rad[id] + 1, qty, elt3dedgerad);
    }
    else
      continue;

    if ((*pMeshData).coldata[id] != NULL)
    {
      if ((*pMeshData).coldatatype[id] != NULL
	 && ! strcmp ((*pMeshData).coldatatype[id], "col"))
	nevs_data_col_colour ((*pMeshData).coldata[id], qty,
			      (*pMeshData).col[id]);
      else if ((*pMeshData).coldatatype[id] != NULL
	  && ! strncmp ((*pMeshData).coldatatype[id], "ori", 3))
	nevs_data_ori_colour ((*pMeshData).coldata[id], qty,
			      (*pMeshData).colscheme[id], (*pMeshData).col[id]);
      else if ((*pMeshData).coldatatype[id] != NULL
	  && ! strcmp ((*pMeshData).coldatatype[id], "scal"))
      {
	nevs_data_scal_colour ((*pMeshData).coldata[id],
			       qty, (*pMeshData).scalemin[id], (*pMeshData).scalemax[id],
			       (*pMeshData).colscheme[id], (*pMeshData).col[id]);
	double val;
	nevs_data_scal_min ((*pMeshData).coldata[id], qty, (*pMeshData).scalemin[id], &val);
	(*pMeshData).scalemin[id] = ut_realloc_1d_char ((*pMeshData).scalemin[id], 100);
	sprintf ((*pMeshData).scalemin[id], "%.15f", val);
	nevs_data_scal_max ((*pMeshData).coldata[id], qty, (*pMeshData).scalemax[id], &val);
	(*pMeshData).scalemax[id] = ut_realloc_1d_char ((*pMeshData).scalemax[id], 100);
	sprintf ((*pMeshData).scalemax[id], "%.15f", val);
      }

      else
	ut_error_reportbug ();
    }
    
    if ((*pMeshData).raddata[id] != NULL)
      nevs_data_rad_radius ((*pMeshData).raddata[id], qty,
			    (*pMeshData).rad[id]);
    
    if ((*pMeshData).coodata[id] != NULL)
    {
      if ((*pMeshData).coodatatype[id] != NULL
	 && ! strcmp ((*pMeshData).coodatatype[id], "coo"))
	nevs_data_coo_coo (Nodes.NodeCoo, (*pMeshData).coodata[id],
			   (*pMeshData).coofact[id], qty, (*pMeshData).coo[id]);
      else if ((*pMeshData).coodatatype[id] != NULL
	  && ! strcmp ((*pMeshData).coodatatype[id], "disp"))
	nevs_data_disp_coo (Nodes.NodeCoo, (*pMeshData).coodata[id],
			    (*pMeshData).coofact[id], qty, (*pMeshData).coo[id]);
      else
	ut_error_reportbug ();
    }
  }

  if ((*pMeshData).coldata[node2elt_id] == NULL)
  {
    ut_free_2d_int ((*pMeshData).col[node2elt_id], (*pMeshData).nodeqty + 1);
    (*pMeshData).col[node2elt_id] = NULL;
  }
  else
  {
    ut_free_2d_int ((*pMeshData).col[elt3d_id], (*pMeshData).elt3dqty + 1);
    (*pMeshData).col[elt3d_id] = NULL;
  }

  return;
}

void
nevs_meshdata_fscanf (struct NODES Nodes, struct MESH Mesh1D, struct MESH Mesh,
                      char* entity, char* type, char* argument, struct MESHDATA* pMeshData)
{
  int id;
  int qty;
  int status;

  neut_meshdata_entity_qty (*pMeshData, entity, &qty);

  ut_print_message (0, 1, "Reading data (%s, %s)...\n", entity, type);

  (*pMeshData).elt3dqty = Mesh.EltQty;
  (*pMeshData).elt1dqty = Mesh1D.EltQty;
  (*pMeshData).nodeqty  = Nodes.NodeQty;

  status = neut_meshdata_entity_id (entity, &id);

  if (status != 0)
    ut_error_reportbug ();

  char** args = NULL;
  int    argqty;
  ut_string_separate (argument, '=', &args, &argqty);

  if (! strcmp (type, "col"))
  {
    (*pMeshData).coldatatype[id] = ut_alloc_1d_char (100);
    char* value = ut_alloc_1d_char (1000);

    nevs_data_colarg_args (argument, (*pMeshData).coldatatype[id], value);

    if (! strcmp ((*pMeshData).coldatatype[id], "col"))
    {
      (*pMeshData).coldata[id] = ut_alloc_2d (qty + 1, 3);
      ut_array_2d_fscanfn_wcard (value, (*pMeshData).coldata[id] + 1,
				   qty, 3, "colour,size");
    }
    else if (! strncmp ((*pMeshData).coldatatype[id], "ori", 3))
    {
      (*pMeshData).coldata[id] = ut_alloc_2d (qty + 1, 3);
      int i;
      double*  tmpd  = ut_alloc_1d     (4);
      int*     tmpi  = ut_alloc_1d_int (6);
      double** tmpdd = ut_alloc_2d     (3, 3);
      FILE* file = ut_file_open (value, "r");

      int qty0;
      if (! strcmp ((*pMeshData).coldatatype[id] + 3, "e" )
       || ! strcmp ((*pMeshData).coldatatype[id] + 3, "ek")
       || ! strcmp ((*pMeshData).coldatatype[id] + 3, "er")
       || ! strcmp ((*pMeshData).coldatatype[id] + 3, "R" ))
	qty0 = 3;
      else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "q"))
	qty0 = 4;
      else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "m"))
	qty0 = 6;
      else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "g"))
	qty0 = 9;
      else
      {
	printf ("(*pMeshData).coldatatype[id] = %s\n", (*pMeshData).coldatatype[id]);
	ut_error_reportbug ();
	abort ();
      }

      ut_file_nbwords_testwmessage (value, qty * qty0);

      for (i = 1; i <= qty; i++)
	if (! strcmp ((*pMeshData).coldatatype[id] + 3, "e"))
	  ol_e_fscanf (file, (*pMeshData).coldata[id][i]);
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "ek"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_ek_e (tmpd, (*pMeshData).coldata[id][i]);
	}
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "er"))
	{
	  ol_e_fscanf (file, tmpd);
	  ol_er_e (tmpd, (*pMeshData).coldata[id][i]);
	}
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "q"))
	{
	  ol_q_fscanf (file, tmpd);
	  ol_q_e (tmpd, (*pMeshData).coldata[id][i]);
	}
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "R"))
	{
	  ol_R_fscanf (file, tmpd);
	  ol_R_e (tmpd, (*pMeshData).coldata[id][i]);
	}
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "m"))
	{
	  ol_m_fscanf (file, tmpi);
	  ol_m_e (tmpi, (*pMeshData).coldata[id][i]);
	}
	else if (! strcmp ((*pMeshData).coldatatype[id] + 3, "g"))
	{
	  ol_g_fscanf (file, tmpdd);
	  ol_g_e (tmpdd, (*pMeshData).coldata[id][i]);
	}
	else
	  ut_error_reportbug ();
    }
    else if (! strcmp ((*pMeshData).coldatatype[id], "scal"))
    {
      (*pMeshData).coldata[id] = ut_alloc_2d (qty + 1, 1);
      ut_array_2d_fscanfn_wcard (value, (*pMeshData).coldata[id] + 1,
				 qty, 1, "numeral,size");
    }
    else
      abort ();

    if (! strcmp (entity, "elt3d"))
    {
      int id2;
      neut_meshdata_entity_id ("node2elt", &id2);

      if ((*pMeshData).coldata[id2] != NULL)
	ut_free_2d ((*pMeshData).coldata[id2], (*pMeshData).nodeqty + 1);
      (*pMeshData).coldata[id2] = NULL;

      ut_free_1d_char ((*pMeshData).coldatatype[id2]);
      (*pMeshData).coldatatype[id2] = NULL;
    }
    
    if (! strcmp (entity, "node2elt"))
    {
      int id2;
      neut_meshdata_entity_id ("elt3d", &id2);

      if ((*pMeshData).coldata[id2] != NULL)
	ut_free_2d ((*pMeshData).coldata[id2], (*pMeshData).elt3dqty + 1);
      (*pMeshData).coldata[id2] = NULL;

      ut_free_1d_char ((*pMeshData).coldatatype[id2]);
      (*pMeshData).coldatatype[id2] = NULL;

      neut_meshdata_entity_id ("elt2d", &id2);

      if ((*pMeshData).coldata[id2] != NULL)
	ut_free_2d ((*pMeshData).coldata[id2], (*pMeshData).elt2dqty + 1);
      (*pMeshData).coldata[id2] = NULL;

      ut_free_1d_char ((*pMeshData).coldatatype[id2]);
      (*pMeshData).coldatatype[id2] = NULL;
    }

    ut_free_1d_char (value);
  }
  else if (! strcmp (type, "rad"))
  {
    (*pMeshData).raddatatype[id] = ut_alloc_1d_char (100);

    if (argqty == 1)
      sprintf ((*pMeshData).raddatatype[id], type);
    else
      strcpy ((*pMeshData).raddatatype[id], args[0]);

    if (! strcmp ((*pMeshData).raddatatype[id], "rad"))
    {
      (*pMeshData).raddata[id] = ut_alloc_2d (qty + 1, 3);
      ut_array_2d_fscanfn_wcard (args[0], (*pMeshData).raddata[id] + 1,
				 qty, 1, "numeral,size");
    }
    else
      abort ();
  }
  else if (! strcmp (type, "colscheme"))
  {
    (*pMeshData).colscheme[id] = ut_alloc_1d_char (100);
    sprintf ((*pMeshData).colscheme[id], argument);
  }
  else if (! strcmp (type, "scalemin"))
  {
    (*pMeshData).scalemin[id] = ut_alloc_1d_char (100);
    sprintf ((*pMeshData).scalemin[id], argument);
  }
  else if (! strcmp (type, "scalemax"))
  {
    (*pMeshData).scalemax[id] = ut_alloc_1d_char (100);
    sprintf ((*pMeshData).scalemax[id], argument);
  }
  else if (! strcmp (type, "scaleticks"))
  {
    (*pMeshData).scaleticks[id] = ut_alloc_1d_char (100);
    sprintf ((*pMeshData).scaleticks[id], argument);
  }
  else if (! strcmp (type, "coo"))
  {
    (*pMeshData).coodatatype[id] = ut_alloc_1d_char (100);

    if (argqty == 1)
      sprintf ((*pMeshData).coodatatype[id], type);
    else
      strcpy ((*pMeshData).coodatatype[id], args[0]);

    if ((*pMeshData).coodata[id] == NULL)
      (*pMeshData).coodata[id] = ut_alloc_2d (qty + 1, 3);

    if (! strcmp ((*pMeshData).coodatatype[id], "coo"))
      ut_array_2d_fscanfn_wcard (args[0], (*pMeshData).coodata[id] + 1,
				 qty, 3, "size");
    else if (! strcmp ((*pMeshData).coodatatype[id], "disp"))
      ut_array_2d_fscanfn_wcard (args[1], (*pMeshData).coodata[id] + 1,
				 qty, 3, "size");
    else
      abort ();
  }
  else if (! strcmp (type, "coofact"))
    sscanf (args[0], "%lf", &((*pMeshData).coofact[id]));
  else
    ut_error_reportbug ();

  if (strlen (entity) >= 5 && ! strncmp (entity, "elset", 5))
    nevs_meshdata_fscanf_set2entity (Mesh1D, Mesh, entity, type, pMeshData);

  return;
}
