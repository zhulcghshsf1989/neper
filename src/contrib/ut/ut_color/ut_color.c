/* Copyright (C) 2007-2009, 2012 Romain Quey */
/* see the COPYING file in the top-level directory.*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "ut_color.h"
#include "ut.h"

int
ut_color_name_valid (char *name)
{
  int status;
  int* c = ut_alloc_1d_int (3);

  status = ut_color_name_rgb (name, c);
  
  ut_free_1d_int (c);

  return (status == -1) ? 0 : 1;
}

int
ut_color_name_rgb (char *name, int *c)
{
  return name2rgb (name, c);
}


int
name2rgb (char *name, int *c)
{
  int i;
  int res = 0;
  char** parts = NULL;
  int qty;

  if (name[0] >= '0' && name[0] <= '9')
  {
    if (strchr (name, ',') != NULL)
      ut_string_separate (name, ',', &parts, &qty);
    else if (strchr (name, '/') != NULL)
      ut_string_separate (name, '/', &parts, &qty);
    else if (strchr (name, '|') != NULL)
      ut_string_separate (name, '|', &parts, &qty);
    else if (strchr (name, '.') != NULL)
      ut_string_separate (name, '.', &parts, &qty);
    else if (strchr (name, ':') != NULL)
      ut_string_separate (name, ':', &parts, &qty);
    else if (strchr (name, ';') != NULL)
      ut_string_separate (name, ';', &parts, &qty);
    else
      res = -1;

    if (qty != 3)
      res = -1;
    else
      for (i = 0; i < 3; i++)
	sscanf (parts[i], "%d", &(c[i]));
    
    ut_free_2d_char (parts, qty);
  }
  else if (strcmp (name, "aliceblue") == 0)
  {
    c[0] = 240;
    c[1] = 248;
    c[2] = 255;
  }
  else if (strcmp (name, "antiquewhite") == 0)
  {
    c[0] = 250;
    c[1] = 235;
    c[2] = 215;
  }
  else if (strcmp (name, "aqua") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "aquamarine") == 0)
  {
    c[0] = 127;
    c[1] = 255;
    c[2] = 212;
  }
  else if (strcmp (name, "azure") == 0)
  {
    c[0] = 240;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "beige") == 0)
  {
    c[0] = 245;
    c[1] = 245;
    c[2] = 220;
  }
  else if (strcmp (name, "bisque") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 196;
  }
  else if (strcmp (name, "black") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "blanchedalmond") == 0)
  {
    c[0] = 255;
    c[1] = 235;
    c[2] = 205;
  }
  else if (strcmp (name, "blue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "blueviolet") == 0)
  {
    c[0] = 138;
    c[1] = 43;
    c[2] = 226;
  }
  else if (strcmp (name, "brown") == 0)
  {
    c[0] = 165;
    c[1] = 42;
    c[2] = 42;
  }
  else if (strcmp (name, "burlywood") == 0)
  {
    c[0] = 222;
    c[1] = 184;
    c[2] = 135;
  }
  else if (strcmp (name, "cadetblue") == 0)
  {
    c[0] = 95;
    c[1] = 158;
    c[2] = 160;
  }
  else if (strcmp (name, "chartreuse") == 0)
  {
    c[0] = 127;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "chocolate") == 0)
  {
    c[0] = 210;
    c[1] = 105;
    c[2] = 30;
  }
  else if (strcmp (name, "coral") == 0)
  {
    c[0] = 255;
    c[1] = 127;
    c[2] = 80;
  }
  else if (strcmp (name, "cornflowerblue") == 0)
  {
    c[0] = 100;
    c[1] = 149;
    c[2] = 237;
  }
  else if (strcmp (name, "cornsilk") == 0)
  {
    c[0] = 255;
    c[1] = 248;
    c[2] = 220;
  }
  else if (strcmp (name, "crimson") == 0)
  {
    c[0] = 220;
    c[1] = 20;
    c[2] = 60;
  }
  else if (strcmp (name, "cyan") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "darkblue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 139;
  }
  else if (strcmp (name, "darkcyan") == 0)
  {
    c[0] = 0;
    c[1] = 139;
    c[2] = 139;
  }
  else if (strcmp (name, "darkgoldenrod") == 0)
  {
    c[0] = 184;
    c[1] = 134;
    c[2] = 11;
  }
  else if (strcmp (name, "darkgray") == 0)
  {
    c[0] = 64;
    c[1] = 64;
    c[2] = 64;
  }
  else if (strcmp (name, "darkgreen") == 0)
  {
    c[0] = 0;
    c[1] = 100;
    c[2] = 0;
  }
  else if (strcmp (name, "darkgrey") == 0)
  {
    c[0] = 64;
    c[1] = 64;
    c[2] = 64;
  }
  else if (strcmp (name, "darkkhaki") == 0)
  {
    c[0] = 189;
    c[1] = 183;
    c[2] = 107;
  }
  else if (strcmp (name, "darkmagenta") == 0)
  {
    c[0] = 139;
    c[1] = 0;
    c[2] = 139;
  }
  else if (strcmp (name, "darkolivegreen") == 0)
  {
    c[0] = 85;
    c[1] = 107;
    c[2] = 47;
  }
  else if (strcmp (name, "darkorange") == 0)
  {
    c[0] = 255;
    c[1] = 140;
    c[2] = 0;
  }
  else if (strcmp (name, "darkorchid") == 0)
  {
    c[0] = 153;
    c[1] = 50;
    c[2] = 204;
  }
  else if (strcmp (name, "darkred") == 0)
  {
    c[0] = 139;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "darksalmon") == 0)
  {
    c[0] = 233;
    c[1] = 150;
    c[2] = 122;
  }
  else if (strcmp (name, "darkseagreen") == 0)
  {
    c[0] = 143;
    c[1] = 188;
    c[2] = 143;
  }
  else if (strcmp (name, "darkslateblue") == 0)
  {
    c[0] = 72;
    c[1] = 61;
    c[2] = 139;
  }
  else if (strcmp (name, "darkslategray") == 0)
  {
    c[0] = 47;
    c[1] = 79;
    c[2] = 79;
  }
  else if (strcmp (name, "darkslategrey") == 0)
  {
    c[0] = 47;
    c[1] = 79;
    c[2] = 79;
  }
  else if (strcmp (name, "darkturquoise") == 0)
  {
    c[0] = 0;
    c[1] = 206;
    c[2] = 209;
  }
  else if (strcmp (name, "darkviolet") == 0)
  {
    c[0] = 148;
    c[1] = 0;
    c[2] = 211;
  }
  else if (strcmp (name, "deeppink") == 0)
  {
    c[0] = 255;
    c[1] = 20;
    c[2] = 147;
  }
  else if (strcmp (name, "deepskyblue") == 0)
  {
    c[0] = 0;
    c[1] = 191;
    c[2] = 255;
  }
  else if (strcmp (name, "dimgray") == 0)
  {
    c[0] = 105;
    c[1] = 105;
    c[2] = 105;
  }
  else if (strcmp (name, "dimgrey") == 0)
  {
    c[0] = 105;
    c[1] = 105;
    c[2] = 105;
  }
  else if (strcmp (name, "dodgerblue") == 0)
  {
    c[0] = 30;
    c[1] = 144;
    c[2] = 255;
  }
  else if (strcmp (name, "firebrick") == 0)
  {
    c[0] = 178;
    c[1] = 34;
    c[2] = 34;
  }
  else if (strcmp (name, "floralwhite") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 240;
  }
  else if (strcmp (name, "forestgreen") == 0)
  {
    c[0] = 34;
    c[1] = 139;
    c[2] = 34;
  }
  else if (strcmp (name, "fuchsia") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "gainsboro") == 0)
  {
    c[0] = 220;
    c[1] = 220;
    c[2] = 220;
  }
  else if (strcmp (name, "ghostwhite") == 0)
  {
    c[0] = 248;
    c[1] = 248;
    c[2] = 255;
  }
  else if (strcmp (name, "gold") == 0)
  {
    c[0] = 255;
    c[1] = 215;
    c[2] = 0;
  }
  else if (strcmp (name, "goldenrod") == 0)
  {
    c[0] = 218;
    c[1] = 165;
    c[2] = 32;
  }
  else if (strcmp (name, "gray") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "grey") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "green") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "greenyellow") == 0)
  {
    c[0] = 173;
    c[1] = 255;
    c[2] = 47;
  }
  else if (strcmp (name, "honeydew") == 0)
  {
    c[0] = 240;
    c[1] = 255;
    c[2] = 240;
  }
  else if (strcmp (name, "hotpink") == 0)
  {
    c[0] = 255;
    c[1] = 105;
    c[2] = 180;
  }
  else if (strcmp (name, "indianred") == 0)
  {
    c[0] = 205;
    c[1] = 92;
    c[2] = 92;
  }
  else if (strcmp (name, "indigo") == 0)
  {
    c[0] = 75;
    c[1] = 0;
    c[2] = 130;
  }
  else if (strcmp (name, "ivory") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 240;
  }
  else if (strcmp (name, "khaki") == 0)
  {
    c[0] = 240;
    c[1] = 230;
    c[2] = 140;
  }
  else if (strcmp (name, "lavender") == 0)
  {
    c[0] = 230;
    c[1] = 230;
    c[2] = 250;
  }
  else if (strcmp (name, "lavenderblush") == 0)
  {
    c[0] = 255;
    c[1] = 240;
    c[2] = 245;
  }
  else if (strcmp (name, "lawngreen") == 0)
  {
    c[0] = 124;
    c[1] = 252;
    c[2] = 0;
  }
  else if (strcmp (name, "lemonchiffon") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 205;
  }
  else if (strcmp (name, "lightblue") == 0)
  {
    c[0] = 173;
    c[1] = 216;
    c[2] = 230;
  }
  else if (strcmp (name, "lightcoral") == 0)
  {
    c[0] = 240;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "lightcyan") == 0)
  {
    c[0] = 224;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "lightgoldenrodyellow") == 0)
  {
    c[0] = 250;
    c[1] = 250;
    c[2] = 210;
  }
  else if (strcmp (name, "lightgray") == 0)
  {
    c[0] = 211;
    c[1] = 211;
    c[2] = 211;
  }
  else if (strcmp (name, "lightgreen") == 0)
  {
    c[0] = 144;
    c[1] = 238;
    c[2] = 144;
  }
  else if (strcmp (name, "lightgrey") == 0)
  {
    c[0] = 211;
    c[1] = 211;
    c[2] = 211;
  }
  else if (strcmp (name, "lightpink") == 0)
  {
    c[0] = 255;
    c[1] = 182;
    c[2] = 193;
  }
  else if (strcmp (name, "lightsalmon") == 0)
  {
    c[0] = 255;
    c[1] = 160;
    c[2] = 122;
  }
  else if (strcmp (name, "lightseagreen") == 0)
  {
    c[0] = 32;
    c[1] = 178;
    c[2] = 170;
  }
  else if (strcmp (name, "lightskyblue") == 0)
  {
    c[0] = 135;
    c[1] = 206;
    c[2] = 250;
  }
  else if (strcmp (name, "lightslategray") == 0)
  {
    c[0] = 119;
    c[1] = 136;
    c[2] = 153;
  }
  else if (strcmp (name, "lightslategrey") == 0)
  {
    c[0] = 119;
    c[1] = 136;
    c[2] = 153;
  }
  else if (strcmp (name, "lightsteelblue") == 0)
  {
    c[0] = 176;
    c[1] = 196;
    c[2] = 222;
  }
  else if (strcmp (name, "lightyellow") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 224;
  }
  else if (strcmp (name, "lime") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "limegreen") == 0)
  {
    c[0] = 50;
    c[1] = 205;
    c[2] = 50;
  }
  else if (strcmp (name, "linen") == 0)
  {
    c[0] = 250;
    c[1] = 240;
    c[2] = 230;
  }
  else if (strcmp (name, "magenta") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 255;
  }
  else if (strcmp (name, "maroon") == 0)
  {
    c[0] = 128;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "mediumaquamarine") == 0)
  {
    c[0] = 102;
    c[1] = 205;
    c[2] = 170;
  }
  else if (strcmp (name, "mediumblue") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 205;
  }
  else if (strcmp (name, "mediumorchid") == 0)
  {
    c[0] = 186;
    c[1] = 85;
    c[2] = 211;
  }
  else if (strcmp (name, "mediumpurple") == 0)
  {
    c[0] = 147;
    c[1] = 112;
    c[2] = 219;
  }
  else if (strcmp (name, "mediumseagreen") == 0)
  {
    c[0] = 60;
    c[1] = 179;
    c[2] = 113;
  }
  else if (strcmp (name, "mediumslateblue") == 0)
  {
    c[0] = 123;
    c[1] = 104;
    c[2] = 238;
  }
  else if (strcmp (name, "mediumspringgreen") == 0)
  {
    c[0] = 0;
    c[1] = 250;
    c[2] = 154;
  }
  else if (strcmp (name, "mediumturquoise") == 0)
  {
    c[0] = 72;
    c[1] = 209;
    c[2] = 204;
  }
  else if (strcmp (name, "mediumvioletred") == 0)
  {
    c[0] = 199;
    c[1] = 21;
    c[2] = 133;
  }
  else if (strcmp (name, "midnightblue") == 0)
  {
    c[0] = 25;
    c[1] = 25;
    c[2] = 112;
  }
  else if (strcmp (name, "mintcream") == 0)
  {
    c[0] = 245;
    c[1] = 255;
    c[2] = 250;
  }
  else if (strcmp (name, "mistyrose") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 225;
  }
  else if (strcmp (name, "moccasin") == 0)
  {
    c[0] = 255;
    c[1] = 228;
    c[2] = 181;
  }
  else if (strcmp (name, "navajowhite") == 0)
  {
    c[0] = 255;
    c[1] = 222;
    c[2] = 173;
  }
  else if (strcmp (name, "navy") == 0)
  {
    c[0] = 0;
    c[1] = 0;
    c[2] = 128;
  }
  else if (strcmp (name, "oldlace") == 0)
  {
    c[0] = 253;
    c[1] = 245;
    c[2] = 230;
  }
  else if (strcmp (name, "olive") == 0)
  {
    c[0] = 128;
    c[1] = 128;
    c[2] = 0;
  }
  else if (strcmp (name, "olivedrab") == 0)
  {
    c[0] = 107;
    c[1] = 142;
    c[2] = 35;
  }
  else if (strcmp (name, "orange") == 0)
  {
    c[0] = 255;
    c[1] = 165;
    c[2] = 0;
  }
  else if (strcmp (name, "orangered") == 0)
  {
    c[0] = 255;
    c[1] = 69;
    c[2] = 0;
  }
  else if (strcmp (name, "orchid") == 0)
  {
    c[0] = 218;
    c[1] = 112;
    c[2] = 214;
  }
  else if (strcmp (name, "palegoldenrod") == 0)
  {
    c[0] = 238;
    c[1] = 232;
    c[2] = 170;
  }
  else if (strcmp (name, "palegreen") == 0)
  {
    c[0] = 152;
    c[1] = 251;
    c[2] = 152;
  }
  else if (strcmp (name, "paleturquoise") == 0)
  {
    c[0] = 175;
    c[1] = 238;
    c[2] = 238;
  }
  else if (strcmp (name, "palevioletred") == 0)
  {
    c[0] = 219;
    c[1] = 112;
    c[2] = 147;
  }
  else if (strcmp (name, "papayawhip") == 0)
  {
    c[0] = 255;
    c[1] = 239;
    c[2] = 213;
  }
  else if (strcmp (name, "peachpuff") == 0)
  {
    c[0] = 255;
    c[1] = 218;
    c[2] = 185;
  }
  else if (strcmp (name, "peru") == 0)
  {
    c[0] = 205;
    c[1] = 133;
    c[2] = 63;
  }
  else if (strcmp (name, "pink") == 0)
  {
    c[0] = 255;
    c[1] = 192;
    c[2] = 203;
  }
  else if (strcmp (name, "plum") == 0)
  {
    c[0] = 221;
    c[1] = 160;
    c[2] = 221;
  }
  else if (strcmp (name, "powderblue") == 0)
  {
    c[0] = 176;
    c[1] = 224;
    c[2] = 230;
  }
  else if (strcmp (name, "purple") == 0)
  {
    c[0] = 128;
    c[1] = 0;
    c[2] = 128;
  }
  else if (strcmp (name, "red") == 0)
  {
    c[0] = 255;
    c[1] = 0;
    c[2] = 0;
  }
  else if (strcmp (name, "rosybrown") == 0)
  {
    c[0] = 188;
    c[1] = 143;
    c[2] = 143;
  }
  else if (strcmp (name, "royalblue") == 0)
  {
    c[0] = 65;
    c[1] = 105;
    c[2] = 225;
  }
  else if (strcmp (name, "saddlebrown") == 0)
  {
    c[0] = 139;
    c[1] = 69;
    c[2] = 19;
  }
  else if (strcmp (name, "salmon") == 0)
  {
    c[0] = 250;
    c[1] = 128;
    c[2] = 114;
  }
  else if (strcmp (name, "sandybrown") == 0)
  {
    c[0] = 244;
    c[1] = 164;
    c[2] = 96;
  }
  else if (strcmp (name, "seagreen") == 0)
  {
    c[0] = 46;
    c[1] = 139;
    c[2] = 87;
  }
  else if (strcmp (name, "seashell") == 0)
  {
    c[0] = 255;
    c[1] = 245;
    c[2] = 238;
  }
  else if (strcmp (name, "sienna") == 0)
  {
    c[0] = 160;
    c[1] = 82;
    c[2] = 45;
  }
  else if (strcmp (name, "silver") == 0)
  {
    c[0] = 192;
    c[1] = 192;
    c[2] = 192;
  }
  else if (strcmp (name, "skyblue") == 0)
  {
    c[0] = 135;
    c[1] = 206;
    c[2] = 235;
  }
  else if (strcmp (name, "slateblue") == 0)
  {
    c[0] = 106;
    c[1] = 90;
    c[2] = 205;
  }
  else if (strcmp (name, "slategray") == 0)
  {
    c[0] = 112;
    c[1] = 128;
    c[2] = 144;
  }
  else if (strcmp (name, "slategrey") == 0)
  {
    c[0] = 112;
    c[1] = 128;
    c[2] = 144;
  }
  else if (strcmp (name, "snow") == 0)
  {
    c[0] = 255;
    c[1] = 250;
    c[2] = 250;
  }
  else if (strcmp (name, "springgreen") == 0)
  {
    c[0] = 0;
    c[1] = 255;
    c[2] = 127;
  }
  else if (strcmp (name, "steelblue") == 0)
  {
    c[0] = 70;
    c[1] = 130;
    c[2] = 180;
  }
  else if (strcmp (name, "tan") == 0)
  {
    c[0] = 210;
    c[1] = 180;
    c[2] = 140;
  }
  else if (strcmp (name, "teal") == 0)
  {
    c[0] = 0;
    c[1] = 128;
    c[2] = 128;
  }
  else if (strcmp (name, "thistle") == 0)
  {
    c[0] = 216;
    c[1] = 191;
    c[2] = 216;
  }
  else if (strcmp (name, "tomato") == 0)
  {
    c[0] = 255;
    c[1] = 99;
    c[2] = 71;
  }
  else if (strcmp (name, "turquoise") == 0)
  {
    c[0] = 64;
    c[1] = 224;
    c[2] = 208;
  }
  else if (strcmp (name, "violet") == 0)
  {
    c[0] = 238;
    c[1] = 130;
    c[2] = 238;
  }
  else if (strcmp (name, "wheat") == 0)
  {
    c[0] = 245;
    c[1] = 222;
    c[2] = 179;
  }
  else if (strcmp (name, "white") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 255;
  }
  else if (strcmp (name, "whitesmoke") == 0)
  {
    c[0] = 245;
    c[1] = 245;
    c[2] = 245;
  }
  else if (strcmp (name, "yellow") == 0)
  {
    c[0] = 255;
    c[1] = 255;
    c[2] = 0;
  }
  else if (strcmp (name, "yellowgreen") == 0)
  {
    c[0] = 154;
    c[1] = 205;
    c[2] = 50;
  }
  else
    res = -1;
  
  return res;
}

void
rgb_intensity (int *rgb, double intensity, int *rgb2)
{
  int i;

  /* intensity = 0 -> black */
  /* intensity = 1 -> white */
  /* intensity = 0.5 -> original color */
  if (intensity < 0)
    intensity = 0;
  if (intensity > 1)
    intensity = 1;

  if (intensity > -1e-6 && intensity < 0.5)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] = (int) (2 * intensity * rgb[i]);
  }
  else if (intensity >= 0.5 && intensity < 1 + 1e-6)
  {
    for (i = 0; i < 3; i++)
      rgb2[i] =
	(int) (2 * (255 - (double) rgb[i]) * intensity - 255 +
	       2 * (double) rgb[i]);

  }

  return;
}


int
ut_color_scheme_val_color (char* scheme, double min, double max,
			   double v, int* col)
{
  int i, pos;
  double pos2;
  char** color = NULL;
  int colqty;
  double* val = NULL;
  int* col0 = ut_alloc_1d_int (3);
  int* col1 = ut_alloc_1d_int (3);
  int status = 0;

  if (scheme != NULL)
    ut_string_separate (scheme, ',', &color, &colqty);
  else
    ut_string_separate ("blue,cyan,yellow,red", ',', &color, &colqty);

  val = ut_alloc_1d (colqty);
  for (i = 0; i < colqty; i++)
    val[i] = min + (max - min) * i / (colqty - 1);

  pos = ut_array_1d_valuepos (val, colqty, v);

  if (pos == -1)
  {
    status = -1;

    if (v < val[0])
      name2rgb (color[0], col);
    else
      name2rgb (color[colqty - 1], col);
  }
  else
  {
    if (pos < colqty - 1)
    {
      name2rgb (color[pos], col0);
      name2rgb (color[pos + 1], col1);
      if (val[pos + 1] - val[pos] != 0)
	pos2 = (v - val[pos]) / (val[pos + 1] - val[pos]);
      else
	pos2 = 0.5;

      for (i = 0; i < 3; i++)
	col[i] = ut_num_d2ri (col0[i] + pos2 * (col1[i] - col0[i]));
    }
    else
      name2rgb (color[pos], col);
  }

  ut_free_1d_int (col0);
  ut_free_1d_int (col1);
  ut_free_2d_char (color, colqty);
  ut_free_1d (val);

  return status;
}
