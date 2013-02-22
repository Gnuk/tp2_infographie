#ifndef BEZIER_H
#define BEZIER_H

#include "geometry.h"

typedef struct bezier_curve3d { 
  int nb_points;
  point3d_cell controles[];
} *bezier_curve3d;

typedef struct bezier_curve1d { 
  int nb_points;
  double controles[];
} *bezier_curve1d;

void calcule3d(point3d result, bezier_curve3d c, double t);
double calcule1d(bezier_curve1d c, double t);

#endif
