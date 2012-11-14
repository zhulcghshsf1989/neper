/* This file is part of the 'neper' program. */
/* Copyright (C) 2003-2012, Romain Quey. */
/* See the COPYING file in the top-level directory. */

extern int  nevs_data_string_entity_type (char*, char*, char*);
extern void nevs_data_entity_type_dataid (struct DATA**, int*, char*, char*, int*);
extern void nevs_data_scal_color (struct DATA*);
extern void nevs_data_col_colour (double**, int, int**);
extern void nevs_data_ori_colour (double**, int, char*, int**);
extern void nevs_data_rad_radius (double**, int, double*);
extern void nevs_data_scal_min (double**, int, char*, double*);
extern void nevs_data_scal_max (double**, int, char*, double*);
extern void nevs_data_scal_colour (double**, int, char*, char*, char*, int**);
extern int nevs_data_type_size (char*, int*);

extern void nevs_data_coo_coo (double**, double**, double, int, double**);
extern void nevs_data_disp_coo (double**, double**, double, int, double**);

extern void nevs_data_colarg_args (char*, char*, char*);
