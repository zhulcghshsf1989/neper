/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_print.h"

void
nevs_print (char** expargv, int* pi,
            struct PRINT* pPrint,
            struct TESS Tess,   struct TESSDATA TessData,
	    struct NODES Nodes, struct MESH Mesh0D,
	    struct MESH Mesh1D, struct MESH Mesh2D,
	    struct MESH Mesh3D, struct MESHDATA MeshData,
	    int SQty, struct NODES* SNodes, struct MESH* SMesh2D,
	    struct MESHDATA* SMeshData)
{
  FILE* file = NULL;
  char* filename = ut_alloc_1d_char (strlen (expargv[(*pi) + 1]) + 10);
  char* filename2 = ut_alloc_1d_char (strlen (expargv[(*pi) + 1]) + 100);
  char* command = ut_alloc_1d_char (1000);
  int i;
  int node2elt_id;
  
  neut_meshdata_entity_id ("node2elt", &node2elt_id);

  (*pi)++;
  sprintf (filename, "%s.pov", expargv[(*pi)]);
    
  ut_print_message (0, 1, "Printing image ...\n");
  file = ut_file_open (filename, "w");
  nevs_print_header (file, (*pPrint));

  // tessellation ------------------------------------------------------
  if ((*pPrint).showtess == 1)
  {
    ut_print_message (0, 2, "Printing tessellation ...\n");
    nevs_print_tess (file, (*pPrint), Tess, TessData);
  }

  // mesh --------------------------------------------------------------
  if ((*pPrint).showmesh == 1)
  {
    ut_print_message (0, 2, "Printing mesh ...\n");
    nevs_print_mesh (file, (*pPrint), Nodes, Mesh0D,
		     Mesh1D, Mesh2D, Mesh3D, MeshData);
  }

  if (SQty > 0 && (*pPrint).showslice != NULL && strlen ((*pPrint).showslice) != 0)
  {
    int node2elt_id, elt2d_id;
    neut_meshdata_entity_id ("node2elt", &node2elt_id);
    neut_meshdata_entity_id ("elt2d", &elt2d_id);
    
    for (i = 0; i < SQty; i++)
    {
      if (SMeshData[i].col[node2elt_id] != NULL)
	nevs_print_mesh2d (file, SNodes[i], SMesh2D[i], *pPrint, 
			   SMeshData[i].col[node2elt_id], "node");
      else if (SMeshData[i].col[elt2d_id] != NULL)
	nevs_print_mesh2d (file, SNodes[i], SMesh2D[i], *pPrint, 
			   SMeshData[i].col[elt2d_id], "elt");
    }
  }

  nevs_print_foot (file, *pPrint);
  ut_file_close (file, filename, "w");


  if (ut_string_inlist ((*pPrint).format, ',', "png"))
    nevs_print_pov2png (filename, (*pPrint).imagewidth,
			(*pPrint).imageheight, (*pPrint).imageantialias, 2);

  if (ut_string_inlist ((*pPrint).format, ',', "pov") == 0)
    remove (filename);

  ut_print_message (0, 1, "Printing scale ...\n");
  char* ent = ut_alloc_1d_char (100);

  int idmax;
  neut_meshdata_idmax (&idmax);

  for (i = 0; i <= idmax; i++)
    if (MeshData.coldatatype[i] != NULL && ! strcmp (MeshData.coldatatype[i], "scal"))
    {
      neut_meshdata_id_entity (i, ent);

      ut_print_message (0, 2, "Printing scale for %s...\n", ent);

      sprintf (filename2, "%s-scale%s.pov", expargv[(*pi)], ent);

      file = ut_file_open (filename2, "w");
      nevs_print_scale (file, MeshData.colscheme[i], MeshData.scalemin[i], MeshData.scalemax[i], MeshData.scaleticks[i]);
      ut_file_close (file, filename2, "w");
  
      if (ut_string_inlist ((*pPrint).format, ',', "png"))
	nevs_print_pov2png (filename2, 0.3*(*pPrint).imageheight,
			    (*pPrint).imageheight, (*pPrint).imageantialias, 3);

      if (ut_string_inlist ((*pPrint).format, ',', "pov") == 0)
	remove (filename2);
    }
  
  neut_tessdata_idmax (&idmax);

  for (i = 0; i <= idmax; i++)
    if (TessData.coldatatype[i] != NULL && ! strcmp (TessData.coldatatype[i], "scal"))
    {
      neut_meshdata_id_entity (i, ent);

      ut_print_message (0, 2, "Printing scale for %s...\n", ent);

      sprintf (filename2, "%s-scale%s.pov", expargv[(*pi)], ent);

      file = ut_file_open (filename2, "w");
      nevs_print_scale (file, TessData.colscheme[i], TessData.scalemin[i], TessData.scalemax[i], TessData.scaleticks[i]);
      ut_file_close (file, filename2, "w");
  
      if (ut_string_inlist ((*pPrint).format, ',', "png"))
	nevs_print_pov2png (filename2, 0.3*(*pPrint).imageheight,
			    (*pPrint).imageheight, (*pPrint).imageantialias, 3);

      if (ut_string_inlist ((*pPrint).format, ',', "pov") == 0)
	remove (filename2);
    }

  ut_free_1d_char (ent);


  ut_free_1d_char (command);
  ut_free_1d_char (filename);
  ut_free_1d_char (filename2);

  return;
}
