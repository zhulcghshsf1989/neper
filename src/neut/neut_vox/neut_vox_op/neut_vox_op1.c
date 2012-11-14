/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "neut_vox_op_lcl.h"

void
neut_vox_set_zero (struct VOX* pVox)
{
  (*pVox).version = NULL;
  (*pVox).format = NULL;
  (*pVox).size = ut_alloc_1d_int (3);
  (*pVox).vsize = ut_alloc_1d (3);
  (*pVox).VoxPoly = NULL;
  (*pVox).PolyQty = 0;

  return;
}
  
void
neut_vox_free (struct VOX* pVox)
{
  ut_free_1d_char ((*pVox).version);
  ut_free_1d_char ((*pVox).format);
  ut_free_3d_int  ((*pVox).VoxPoly, (*pVox).size[0], (*pVox).size[1]);
  ut_free_1d_int  ((*pVox).size);
  ut_free_1d      ((*pVox).vsize);

  neut_vox_set_zero (pVox);

  return;
}

void
neut_vox_memcpy (struct VOX Vox1, struct VOX* pVox2)
{
  neut_vox_free (pVox2);

  (*pVox2).version = ut_alloc_1d_char (strlen (Vox1.version) + 1);
  strcpy ((*pVox2).version, Vox1.version);
  (*pVox2).format = ut_alloc_1d_char (strlen (Vox1.format) + 1);
  strcpy ((*pVox2).format, Vox1.format);

  (*pVox2).size = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pVox2).size, 3, Vox1.size);

  (*pVox2).vsize = ut_alloc_1d (3);
  ut_array_1d_memcpy ((*pVox2).vsize, 3, Vox1.vsize);

  (*pVox2).PolyQty = Vox1.PolyQty;
  (*pVox2).VoxPoly = ut_alloc_3d_int ((*pVox2).size[0], (*pVox2).size[1], (*pVox2).size[2]);
  ut_array_3d_int_memcpy ((*pVox2).VoxPoly, (*pVox2).size[0],
                          (*pVox2).size[1], (*pVox2).size[2], Vox1.VoxPoly);

  return;
}

void
neut_vox_memcpy_parms (struct VOX Vox1, struct VOX* pVox2)
{
  neut_vox_free (pVox2);

  (*pVox2).version = ut_alloc_1d_char (strlen (Vox1.version) + 1);
  strcpy ((*pVox2).version, Vox1.version);
  (*pVox2).format = ut_alloc_1d_char (strlen (Vox1.format) + 1);
  strcpy ((*pVox2).format, Vox1.format);

  (*pVox2).size = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pVox2).size, 3, Vox1.size);

  (*pVox2).vsize = ut_alloc_1d (3);
  ut_array_1d_memcpy ((*pVox2).vsize, 3, Vox1.vsize);

  (*pVox2).PolyQty = Vox1.PolyQty;

  return;
}

int
neut_vox_expr_polys (struct VOX Vox, char* expr, int** ppoly, int* ppolyqty)
{
  int i, id;
  FILE* file;
  (*ppolyqty) = 0;

  if (strcmp (expr, "all") == 0)
  {
    (*ppolyqty) = Vox.PolyQty;
    (*ppoly) = ut_alloc_1d_int (*ppolyqty);
    for (i = 0; i < Vox.PolyQty; i++)
      (*ppoly)[i] = i + 1;
  }

  else if (expr[0] == '@')
  {
    file = ut_file_open (expr + 1, "r");
    while (fscanf (file, "%d", &id) != EOF)
    {
      (*ppolyqty)++;
      (*ppoly) = ut_realloc_1d_int (*ppoly, *ppolyqty);
      (*ppoly)[(*ppolyqty) - 1] = id;
    }
    ut_file_close (file, expr + 1, "r");
  }
  else
  {
    int var_qty = 1;
    char **vars = ut_alloc_2d_char (var_qty, 15);
    double *vals = ut_alloc_1d (var_qty);
    int status;
    double res;

    sprintf (vars[0], "id");

    for (i = 1; i <= Vox.PolyQty; i++)
    {
      vals[0] = i;
#ifdef HAVE_LIBMATHEVAL
      status = ut_math_eval (expr, var_qty, vars, vals, &res);
#else
ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
abort ();
#endif
      if (status == -1)
	abort ();
      if (res == 1)
      {
	(*ppolyqty)++;
	(*ppoly) = ut_realloc_1d_int (*ppoly, *ppolyqty);
	(*ppoly)[(*ppolyqty) - 1] = i;
      }
    }

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
  }

  return 0;
}

void
neut_vox_boundingbox (struct VOX Vox, double** bbox)
{
  int i;

  for (i = 0; i < 3; i++)
  {
    bbox[i][0] = 0;
    bbox[i][1] = Vox.size[i] * Vox.vsize[i];
  }

  return;
}

void
neut_vox_scale (struct VOX* pVox, double scale1, double scale2,
                double scale3, char* method)
{
  int i;
  double* scale = ut_alloc_1d (3);

  scale[0] = scale1;
  scale[1] = scale2;
  scale[2] = scale3;

  if (method == NULL)
  {
    for (i = 0; i < 3; i++)
      (*pVox).vsize[i] *= scale[i];
  }
  /*
  else if (! strcmp (method, "gridscale"))
  {
    int* newsize = ut_alloc_1d_int (3);
    double* gridscale = ut_alloc_1d (3);
    double avvsize;
    double* newdomsize = ut_alloc_1d (3);

    printf ("Tihs part is buggy\n");
    for (i = 0; i < 3; i++)
      newdomsize[i] = (*pVox).size[i] * (*pVox).vsize[i] * scale[i];
    double avnewdomsize = ut_array_1d_gmean (newdomsize, 3);

    int voxqty = (*pVox).size[0] * (*pVox).size[1] * (*pVox).size[2];

    avvsize = ut_array_1d_gmean ((*pVox).vsize, 3);

    for (i = 0; i < 3; i++)
      newsize[i]
	= ut_num_d2ri (pow (voxqty, 1./3)
		       * (newdomsize[i] / avnewdomsize)
		       * ((*pVox).vsize[i] / avvsize)
		      );

    for (i = 0; i < 3; i++)
      gridscale[i] = newsize[i] / (*pVox).size[i];

    neut_vox_gridscale (pVox, gridscale[0], gridscale[0], gridscale[2]);
    ut_free_1d (gridscale);
    ut_free_1d_int (newsize);
  }
  */

  return;
}


void
neut_vox_gridscale (struct VOX* pVox, double scale1, double scale2, double scale3)
{
  int i, j, k, l, m, n, qty;
  struct VOX Vox2;
  int** pos = ut_alloc_2d_int (3, 2);
  srand48 (1);
  double* scale = ut_alloc_1d (3);
  scale[0] = scale1;
  scale[1] = scale2;
  scale[2] = scale3;

  neut_vox_set_zero (&Vox2);

  Vox2.version = ut_alloc_1d_char (strlen ((*pVox).version) + 1);
  strcpy (Vox2.version, (*pVox).version);
  Vox2.format = ut_alloc_1d_char (strlen ((*pVox).format) + 1);
  strcpy (Vox2.format, (*pVox).format);

  Vox2.PolyQty = (*pVox).PolyQty;
  
  Vox2.size = ut_alloc_1d_int (3);
  for (i = 0; i < 3; i++)
    Vox2.size[i] = (*pVox).size[i] * scale[i];

  Vox2.vsize = ut_alloc_1d (3);
  for (i = 0; i < 3; i++)
    Vox2.vsize[i] = (*pVox).vsize[i] / ((double) Vox2.size[i] / (double) (*pVox).size[i]);

  Vox2.VoxPoly = ut_alloc_3d_int (Vox2.size[0], Vox2.size[1], Vox2.size[2]);

  for (k = 0; k < Vox2.size[2]; k++)
  {
    pos[2][0] = ((double) k / Vox2.size[2]) * ((*pVox).size[2]);
    pos[2][1] = ((double) (k + 0.99999999) / Vox2.size[2]) * ((*pVox).size[2]);

    for (j = 0; j < Vox2.size[1]; j++)
    {
      pos[1][0] = ((double) j       / (double) Vox2.size[1]) * ((*pVox).size[1]);
      pos[1][1] = ((double) (j + 0.99999999) / (double) Vox2.size[1]) * ((*pVox).size[1]);

      for (i = 0; i < Vox2.size[0]; i++)
      {
	pos[0][0] = ((double) i       / (double) Vox2.size[0]) * ((*pVox).size[0]);
	pos[0][1] = ((double) (i + 0.99999999) / (double) Vox2.size[0]) * ((*pVox).size[0]);

	qty = (pos[2][1] - pos[2][0] + 1) * (pos[1][1] - pos[1][0] + 1) * (pos[0][1] - pos[0][0] + 1);
	int* array = ut_alloc_1d_int (qty);
	int** val = NULL;
	int valqty, qty2;

	qty = 0;
	for (n = pos[2][0]; n <= pos[2][1]; n++)
	  for (m = pos[1][0]; m <= pos[1][1]; m++)
	    for (l = pos[0][0]; l <= pos[0][1]; l++)
	      array[qty++] = (*pVox).VoxPoly[l][m][n];

	ut_array_1d_int_valqty (array, qty, &val, &valqty);

	int* perm = ut_alloc_1d_int (valqty);
	ut_array_1d_int_sort_index (val[1], valqty, perm);
	ut_array_1d_int_reverseelts (perm, valqty);
	ut_array_1d_permutation_int (val[0], valqty, perm);
	ut_array_1d_permutation_int (val[1], valqty, perm);
	ut_free_1d_int (perm);

	if (valqty == 0)
	  ut_error_reportbug ();
	else if (valqty == 1)
	  Vox2.VoxPoly[i][j][k] = val[0][0];
	else
	{
	  qty2 = ut_array_1d_int_nbofthisval (val[1], valqty, val[1][0]);

	  if (qty2 == 1)
	    Vox2.VoxPoly[i][j][k] = val[0][0];
	  else
	    Vox2.VoxPoly[i][j][k] = val[0][(int) (drand48 () * qty2)];
	}

	ut_free_1d_int (array);
	ut_free_2d_int (val, 2);
      }
    }
  }

  neut_vox_free (pVox);
  neut_vox_memcpy (Vox2, pVox);
  neut_vox_free (&Vox2);

  ut_free_2d_int (pos, 3);
  ut_free_1d (scale);

  return;
}

void
neut_vox_segment (struct VOX Vox, int type, struct VOX* pSeg)
{
  int i, j, k, l, m;
  int vox1qty, vox2qty;
  int** vox1 = NULL;
  int** vox2 = NULL;
  int qty;
  int** tmp = NULL;
  int*  pos  = ut_alloc_1d_int (3);

  neut_vox_set_zero (pSeg);
  (*pSeg).version = ut_alloc_1d_char (strlen (Vox.version) + 1);
  strcpy ((*pSeg).version, Vox.version);
  (*pSeg).format = ut_alloc_1d_char (strlen (Vox.format) + 1);
  strcpy ((*pSeg).format, Vox.format);
    
  (*pSeg).size = ut_alloc_1d_int (3);
  ut_array_1d_int_memcpy ((*pSeg).size, 3, Vox.size);

  (*pSeg).VoxPoly = ut_alloc_3d_int ((*pSeg).size[0], (*pSeg).size[1], (*pSeg).size[2]);

  (*pSeg).PolyQty = 0;
  for (k = 0; k < Vox.size[2]; k++)
  {
    for (j = 0; j < Vox.size[1]; j++)
      for (i = 0; i < Vox.size[0]; i++)
	// pixel belongs to the 0 phase and has not been segmented yet
	if (Vox.VoxPoly[i][j][k] != 0 && ut_num_d2ri ((*pSeg).VoxPoly[i][j][k]) == 0)
	{
	  int voxqty = 0;
	  vox1qty = 1;
	  vox1 = ut_alloc_2d_int (1, 3);
	  vox1[0][0] = i;
	  vox1[0][1] = j;
	  vox1[0][2] = k;
	  (*pSeg).PolyQty++;
	  // fflush (stdout);

	  (*pSeg).VoxPoly[i][j][k] = (*pSeg).PolyQty;
	  voxqty++;

	  while (vox1qty > 0)
	  {
	    /*
	    printf ("------------------------ \n");
	    printf ("vox1qty = %d\n", vox1qty);
	    ut_array_2d_int_fprintf (stdout, vox1, vox1qty, 3, "%d");
	    */

	    vox2qty = 0;

	    for (l = 0; l < vox1qty; l++)
	    {
	      // printf ("l = %d/%d\n", l + 1, vox1qty);

	      neut_vox_polys_vox_neighvox (Vox, NULL, 0, vox1[l], type, &tmp, &qty);
	      /*
	      printf ("qty = %d\n", qty);
	      printf ("tmp = \n");
	      ut_array_2d_int_fprintf (stdout, tmp, qty, 3, "%d");
	      */

	      for (m = 0; m < qty; m++)
		if (Vox.VoxPoly[tmp[m][0]][tmp[m][1]][tmp[m][2]] != 0
		 && ut_num_d2ri ((*pSeg).VoxPoly[tmp[m][0]][tmp[m][1]][tmp[m][2]] == 0))
		{
		  // printf ("recording %d %d %d", tmp[m][0], tmp[m][1], tmp[m][2]);
		  // printf (" in vox2 (vox2qty = %d)\n", vox2qty);
		  if (ut_array_2d_int_linepos (vox2, vox2qty, 3, tmp[m]) == -1)
		  {
		    vox2qty++;
		    vox2 = ut_realloc_2d_int_addline (vox2, vox2qty, 3);
		    ut_array_1d_int_memcpy (vox2[vox2qty - 1], 3, tmp[m]);
		  }
		}
	      ut_free_2d_int (tmp, qty);
	      tmp = NULL;
	    }
	    
	    /* 
	    printf ("vox2qty = %d\n", vox2qty);
	    printf ("vox2 =\n");
	    ut_array_2d_int_fprintf (stdout, vox2, vox2qty, 3, "%d");
	    */

	    for (m = 0; m < vox2qty; m++)
	    {
	      (*pSeg).VoxPoly[vox2[m][0]][vox2[m][1]][vox2[m][2]] = (*pSeg).PolyQty;
	      voxqty++;
	    }

	    ut_free_2d_int (vox1, vox1qty);
	    vox1 = NULL;
	    vox1qty = vox2qty;
	    vox1 = ut_alloc_2d_int (vox1qty, 3);
	    ut_array_2d_int_memcpy (vox1, vox1qty, 3, vox2);
	    ut_free_2d_int (vox2, vox2qty);
	    vox2 = NULL;
	    vox2qty = 0;
	  }

	  // printf ("%4d voxels\n", voxqty);
	}
  }

  ut_free_1d_int (pos);

  return;
}

void
neut_vox_clean (struct VOX Vox, int type, struct VOX* pVox2,
                int* pvoxqty, int* ppolyqty)
{
  int id;
  struct VOX Seg;
  int* voxqty = NULL;

  neut_vox_set_zero (&Seg);

  neut_vox_segment (Vox, type, &Seg);

  neut_vox_polys_voxqty (Seg, &voxqty);

  id = 1 + ut_array_1d_max_int_index (voxqty + 1, Seg.PolyQty);

  neut_vox_poly_vox (Seg, id, pVox2);

  if (pvoxqty != NULL)
    (*pvoxqty)  = ut_array_1d_int_sum (voxqty + 1, Seg.PolyQty) - voxqty[id];

  if (ppolyqty != NULL)
    (*ppolyqty) = Seg.PolyQty - 1;

  ut_free_1d_int (voxqty);
  neut_vox_free (&Seg);

  return;
}

void
neut_vox_polys_voxqty (struct VOX Vox, int** pvoxqty)
{
  int i, j, k;

  (*pvoxqty) = ut_alloc_1d_int (Vox.PolyQty + 1);

  for (k = 0; k < Vox.size[2]; k++)
    for (j = 0; j < Vox.size[1]; j++)
      for (i = 0; i < Vox.size[0]; i++)
	(*pvoxqty)[Vox.VoxPoly[i][j][k]]++;

  return;
}

void
neut_vox_poly_vox (struct VOX Vox, int poly, struct VOX* pPolyVox)
{
  int i, j, k;

  neut_vox_memcpy_parms (Vox, pPolyVox);

  (*pPolyVox).PolyQty = 1;
  (*pPolyVox).VoxPoly
    = ut_alloc_3d_int (Vox.size[0], Vox.size[1], Vox.size[2]);

  for (k = 0; k < Vox.size[2]; k++)
    for (j = 0; j < Vox.size[1]; j++)
      for (i = 0; i < Vox.size[0]; i++)
	if (Vox.VoxPoly[i][j][k] == poly)
	  (*pPolyVox).VoxPoly[i][j][k] = poly;

  return;
}

void
neut_vox_poly_vox_neighvox (struct VOX Vox, int poly, int* pos, int type, int*** ppos, int* pqty)
{
  int* array = ut_alloc_1d_int (1);
  array[0] = poly;

  neut_vox_polys_vox_neighvox (Vox, array, 1, pos, type, ppos, pqty);

  ut_free_1d_int (array);

  return;
}

// poly == NULL has a special behaviour
void
neut_vox_polys_vox_neighvox (struct VOX Vox, int* poly, int polyqty, int* pos,
                             int type, int*** ppos, int* pqty)
{
  int i, j, k, x, y, z;
  int** tmp = NULL;
  int qty;

  (*pqty) = 0;
  (*ppos) = NULL;

  if (type == 1)
  {
    tmp = NULL;
    qty = 0;
    int x, y, z;
    for (k = -1; k <= 1; k++)
      for (j = -1; j <= 1; j++)
	for (i = -1; i <= 1; i++)
	{
	  x = pos[0] + i;
	  y = pos[1] + j;
	  z = pos[2] + k;

	  if ((i == 0 && j == 0 && k == 0)
	   || (x < 0 || x > Vox.size[0] - 1)
	   || (y < 0 || y > Vox.size[1] - 1)
	   || (z < 0 || z > Vox.size[2] - 1))
	    continue;
	    
	  tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
	  tmp[qty][0] = x;
	  tmp[qty][1] = y;
	  tmp[qty][2] = z;
	  qty++;
	}
  }
  else if (type == 2)
  {
    tmp = ut_alloc_2d_int (6, 3);
    qty = 0;
    if (pos[0] > 0)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0] - 1;
      tmp[qty][1] = pos[1];
      tmp[qty][2] = pos[2];
      qty++;
    }
    if (pos[0] < Vox.size[0] - 1)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0] + 1;
      tmp[qty][1] = pos[1];
      tmp[qty][2] = pos[2];
      qty++;
    }
    if (pos[1] > 0)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0];
      tmp[qty][1] = pos[1] - 1;
      tmp[qty][2] = pos[2];
      qty++;
    }
    if (pos[1] < Vox.size[1] - 1)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0];
      tmp[qty][1] = pos[1] + 1;
      tmp[qty][2] = pos[2];
      qty++;
    }
    if (pos[2] > 0)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0];
      tmp[qty][1] = pos[1];
      tmp[qty][2] = pos[2] - 1;
      qty++;
    }
    if (pos[2] < Vox.size[2] - 1)
    {
      tmp = ut_realloc_2d_int_addline (tmp, qty + 1, 3);
      tmp[qty][0] = pos[0];
      tmp[qty][1] = pos[1];
      tmp[qty][2] = pos[2] + 1;
      qty++;
    }
  }
  else
    abort ();

  for (i = 0; i < qty; i++)
  {
    x = tmp[i][0];
    y = tmp[i][1];
    z = tmp[i][2];

    if ((poly == NULL && Vox.VoxPoly[x][y][z] != 0)
     || ut_array_1d_int_eltpos (poly, polyqty, Vox.VoxPoly[x][y][z]) != -1)
    {
      (*ppos) = ut_realloc_2d_int_addline (*ppos, ++(*pqty), 3);
      (*ppos)[(*pqty) - 1][0] = x;
      (*ppos)[(*pqty) - 1][1] = y;
      (*ppos)[(*pqty) - 1][2] = z;
    }
  }

  ut_free_2d_int (tmp, qty);

  return;
}
