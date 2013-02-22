#ifndef CONVEX_H
#define CONVEX_H

#include "triangulation.h"
#include "array.h"


half_edge convex_hull(gl_vertex *pts, size_t nb_pts);
half_edge test1();
half_edge test_random(size_t n);

#endif
