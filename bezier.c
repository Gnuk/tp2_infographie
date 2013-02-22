#include <string.h>
#include "geometry.h"
#include "bezier.h"

/**
 * Question 1
 */
void calcule3d(point3d result, bezier_curve3d c, double t) {
	int i, j, max;
	point3d_cell tableau[c->nb_points];
	
	max = c->nb_points-1;
	for(i=0; i < c->nb_points; i++){
		tableau[i] = c->controles[i];
	}
	for(i=max ; i > 0 ; i--){
		for(j=0; j < i; j++){
			bary3d(&tableau[j], (1.0-t),  &tableau[j] ,t, &tableau[j+1]);
		}
	}
	*result = tableau[0];
}

/**
 * Question 1
 */
double calcule1d(bezier_curve1d c, double t) {
  double tableau[c->nb_points];
  int i, j, max;
	for(i=0; i < c->nb_points; i++){
		tableau[i] = c->controles[i];
	}
	max = c->nb_points-1;
	for(i = max ; i > 0 ; i--){
		for(j=0; j<i; j++){
			tableau[j] = (1.0-t) * tableau[j] + t * tableau[j+1];
		}
	}
	return tableau[0];
}

