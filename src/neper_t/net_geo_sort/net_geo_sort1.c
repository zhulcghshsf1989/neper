/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

#include "net_geo_sort.h"

extern void
net_geo_sort (struct GEO* pGeo, char* entity, char* expr)
{
  int i;
  int var_qty;
  char** vars = NULL;
  double* vals = NULL;
  double* res = NULL;
  int status = -1;
  char* expr2 = NULL;
  int knownexpr;
  int* perm = NULL;
  
  if (strcmp (expr, "true") == 0)
    knownexpr =  3;
  else if (strcmp (expr, "-true") == 0)
    knownexpr = -3;
  else if (strcmp (expr, "body") == 0)
    knownexpr =  4;
  else if (strcmp (expr, "-body") == 0)
    knownexpr = -4;
  else
  {
    knownexpr = 0;
#ifdef HAVE_LIBMATHEVAL
    status = ut_math_eval_substitute (expr, &expr2);
#else 
    ut_print_messagewnc (2, 72, "This capability is not available because this version of Neper has not been compiled with libmatheval.");
    abort ();
#endif
  }

  ut_print_message (0, 2, "Sorting %s with expression \"%s\" ...\n",
      entity, expr);
  if (knownexpr == 0 && status == -1)
  {
    ut_print_message (2, 0, "Expression of unknown format.\n");
    abort ();
  }

  if (strcmp (entity, "poly") == 0)
  {
    var_qty = 6;
    vars = ut_alloc_2d_char (var_qty, 5);
    vals = ut_alloc_1d      (var_qty);
    strcpy (vars[0], "cenx");
    strcpy (vars[1], "ceny");
    strcpy (vars[2], "cenz");
    strcpy (vars[3], "true");
    strcpy (vars[4], "body");
    strcpy (vars[5], "vol");

    res = ut_alloc_1d ((*pGeo).PolyQty + 1);
    (*pGeo).PolyId = ut_alloc_1d_int ((*pGeo).PolyQty + 1);
    perm = ut_alloc_1d_int ((*pGeo).PolyQty + 1);
    for (i = 1; i <= (*pGeo).PolyQty; i++)
    {
      // initialize vals for this poly
      vals[0] = (*pGeo).CenterCoo[i][0];
      vals[1] = (*pGeo).CenterCoo[i][1];
      vals[2] = (*pGeo).CenterCoo[i][2];
      vals[3] = (*pGeo).PolyTrue[i];
      vals[4] = (*pGeo).PolyBody[i];
      neut_geo_poly_volume (*pGeo, i, &(vals[5]));

      // calculating function value
      if (knownexpr != 0)
	res[i] = ut_num_sgn (knownexpr) * vals[ut_num_fabs_int(knownexpr)];
#ifdef HAVE_LIBMATHEVAL 
      else
	status = ut_math_eval (expr2, var_qty, vars, vals, &(res[i]));
#endif
    }

    ut_array_1d_sort_index (res + 1, (*pGeo).PolyQty, (*pGeo).PolyId + 1);

    ut_array_1d_sort_index_perm ((*pGeo).PolyId + 1, (*pGeo).PolyQty, perm + 1);
    ut_array_1d_int_addval ((*pGeo).PolyId + 1, (*pGeo).PolyQty, 1, (*pGeo).PolyId + 1);
    ut_array_1d_int_addval (perm + 1, (*pGeo).PolyQty, 1, perm + 1);

    for (i = 1; i <= (*pGeo).PolyQty; i++)
      neut_geo_poly_switch (pGeo, i, perm[i]);

    ut_free_2d_char (vars, var_qty);
    ut_free_1d (vals);
    ut_free_1d (res);
    ut_free_1d_int (perm);
  }

  if (knownexpr == 0)
    ut_free_1d_char (expr2);
  
  return;
}
