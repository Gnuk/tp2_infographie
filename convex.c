#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "geometry.h"
#include "array.h"
#include "triangulation_tools.h"
#include "convex.h"
#include "tiny_gc.h"


double gl_det(gl_vertex* v1, gl_vertex* v2, gl_vertex* v3, gl_vertex* v4) {
  if (v1->vertex_type == GL_DOUBLE) {
    vecteur3d_cell V1, V2, V3;
    vec3d(&V1,v1->coord.a3d,v2->coord.a3d);
    vec3d(&V2,v1->coord.a3d,v3->coord.a3d);
    vec3d(&V3,v1->coord.a3d,v4->coord.a3d);
    
    return det3d(&V1,&V2,&V3);
  } else {
    vecteur3f_cell V1, V2, V3;
    vec3f(&V1,v1->coord.a3f,v2->coord.a3f);
    vec3f(&V2,v1->coord.a3f,v3->coord.a3f);
    vec3f(&V3,v1->coord.a3f,v4->coord.a3f);
    
    return (double) det3f(&V1,&V2,&V3);
  }
}

half_edge remove_visible(half_edge e, gl_vertex *pts) {
  edge_list sides = NULL;
  
  void fn(half_edge f, gl_vertex* v1, gl_vertex* v2, gl_vertex* v3) {
    /*
    printf ("test: ");
    print_vertex(v1);
    print_vertex(v2);
    print_vertex(v3);
    print_vertex(pts);
    printf ("=> %lf \n", gl_det(v1,v2,v3,pts) );
    */

    if (gl_det(v1,v2,v3,pts) > 1e-8) {
      /*
      printf ("remove: ");
      print_vertex(v1);
      print_vertex(v2);
      print_vertex(v3);
      printf ("\n");
      */
      half_edge tmp = f;
      if (tmp->opp->next) sides = cons_edge_list(tmp, sides);
      tmp = tmp->next->opp;
      if (tmp->opp->next) sides = cons_edge_list(tmp, sides);
      tmp = tmp->next->opp;
      if (tmp->opp->next) sides = cons_edge_list(tmp, sides);
      remove_triangle(f);
    }
  }

  iter_triangles(e, fn);

  while(sides) {
    if (sides->head->prev) return sides->head;
    sides = sides -> tail;
  } 

  return NULL;
}

half_edge next_on_side(half_edge e) {
  e = e->opp;
  while(e->next) e = e->next;
  return e;
}

void fill_hole(half_edge e, gl_vertex *pts) {

  half_edge e1 = next_on_side(e);
  gl_vertex *start = e->vertex;

  add_vertex_to_edge(e, pts);

  half_edge e0 = e -> opp -> prev -> opp;
  
  while(e1 -> opp -> vertex != start) {
    close_triangle(e0,e1);
    e0 = e1 -> opp -> prev -> opp;
    e1 = next_on_side(e1);
  }

  fill_triangle(e0,e1,e->next);
}

half_edge convex_hull(gl_vertex *pts, size_t nb_pts) {

  if (nb_pts < 4) {
    fprintf(stderr, "enveloppe for at least four points\n");
  }
 
  if (gl_det(&(pts[0]), &(pts[1]), &(pts[2]), &(pts[3])) > 0.0) {
    gl_vertex tmp = pts[0];
    pts[0] = pts[1];
    pts[1] = tmp;
  }
    
  half_edge e = tetraedron(&(pts[0]), &(pts[1]), &(pts[2]), &(pts[3]));

  size_t i;

  for(i=4;i<nb_pts;i++) {
    half_edge e0 = remove_visible(e,&(pts[i]));
    if (e0) {
      e = e0;
      fill_hole(e,&(pts[i]));
    }
  }

  return(e);
    
}

double randd(double min, double max)
{
  double r=(double)rand()/(double)RAND_MAX;
  return r*(max-min)+min;
}

half_edge test_random(size_t n) {
  gl_vertex *v = GC_malloc(n * sizeof(gl_vertex));
  size_t i;

  for(i=0;i<n;i++) {
    v[i] = *(GLvertex3d(randd(-1.0,1.0),randd(-1.0,1.0),randd(-1.0,1.0)));
  }

  global_pts = v;
  nb_global_pts = n;
  return convex_hull(v,n);
}

half_edge test1() {
  size_t n = 5;
  gl_vertex *v = GC_malloc(n * sizeof(gl_vertex));

  v[0] = *(GLvertex3d(0.0,0.0,0.0));
  v[1] = *(GLvertex3d(1.0,0.0,0.0));
  v[2] = *(GLvertex3d(0.0,1.0,0.0));
  v[3] = *(GLvertex3d(0.0,0.0,1.0));
  v[4] = *(GLvertex3d(1.0,1.0,1.0));

  return convex_hull(v,n);
}
