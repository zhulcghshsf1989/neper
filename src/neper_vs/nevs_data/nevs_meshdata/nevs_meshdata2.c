/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include"nevs_meshdata.h"

void
nevs_meshdata_fscanf_set2entity (struct MESH Mesh1D,
				 struct MESH Mesh, char* entity, char* type,
				 struct MESHDATA* pMeshData)
{
  char* entity2 = ut_alloc_1d_char (100);
  int i, qty, qty2, id, id2;
  int* src = NULL;

  if (! strcmp (entity, "elset3d"))
    sprintf (entity2, "elt3d");
  else if (! strcmp (entity, "elset2d"))
    sprintf (entity2, "elt2d");
  else if (! strcmp (entity, "elset1d"))
    sprintf (entity2, "elt1d");
  else if (! strcmp (entity, "elset3dedge"))
    sprintf (entity2, "elt3dedge");
  else
    ut_error_reportbug ();

  neut_meshdata_entity_id (entity , &id );
  neut_meshdata_entity_id (entity2, &id2);
  
  neut_meshdata_entity_qty (*pMeshData, entity , &qty );
  neut_meshdata_entity_qty (*pMeshData, entity2, &qty2);
      
  if (! strcmp (entity2, "elt3d"))
    src = Mesh.EltElset;
  else if (! strcmp (entity2, "elt3dedge"))
    src = Mesh.EltElset;
  else if (! strcmp (entity2, "elt1d"))
    src = Mesh1D.EltElset;
  else
    abort ();

  if (! strcmp (type, "col"))
  {
    (*pMeshData).coldatatype[id2] = ut_alloc_1d_char (strlen ((*pMeshData).coldatatype[id]) + 1);
    strcpy ((*pMeshData).coldatatype[id2], (*pMeshData).coldatatype[id]);

    (*pMeshData).coldata[id2] = ut_alloc_2d (qty2 + 1, 3);

    for (i = 1; i <= qty2; i++)
      ut_array_1d_memcpy ((*pMeshData).coldata[id2][i], 3, (*pMeshData).coldata[id][src[i]]);
  }
  else if (! strcmp (type, "rad"))
  {
    (*pMeshData).raddatatype[id2] = ut_alloc_1d_char (strlen ((*pMeshData).raddatatype[id]) + 1);
    strcpy ((*pMeshData).raddatatype[id2], (*pMeshData).raddatatype[id]);
    
    (*pMeshData).raddata[id2] = ut_alloc_2d (qty2 + 1, 3);

    for (i = 1; i <= qty2; i++)
      (*pMeshData).raddata[id2][i] = (*pMeshData).raddata[id][src[i]];
  }
  else if (! strcmp (type, "colscheme"))
  {
    (*pMeshData).colscheme[id2] = ut_alloc_1d_char (strlen ((*pMeshData).colscheme[id]) + 1);
    sprintf ((*pMeshData).colscheme[id2], (*pMeshData).colscheme[id]);
  }
  else if (! strcmp (type, "scalemin"))
  {
    (*pMeshData).scalemin[id2] = ut_alloc_1d_char (strlen ((*pMeshData).scalemin[id]) + 1);
    sprintf ((*pMeshData).scalemin[id2], (*pMeshData).scalemin[id]);
  }
  else if (! strcmp (type, "scalemax"))
  {
    (*pMeshData).scalemax[id2] = ut_alloc_1d_char (strlen ((*pMeshData).scalemax[id]) + 1);
    sprintf ((*pMeshData).scalemax[id2], (*pMeshData).scalemax[id]);
  }
  else if (! strcmp (type, "scaleticks"))
  {
    (*pMeshData).scaleticks[id2] = ut_alloc_1d_char (strlen ((*pMeshData).scaleticks[id]) + 1);
    sprintf ((*pMeshData).scaleticks[id2], (*pMeshData).scaleticks[id]);
  }
  else
    ut_error_reportbug ();

  ut_free_1d_char (entity2);

  return;
}
