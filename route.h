#ifndef ROUTE_H
#define ROUTE_H

#ifndef extern 
#define extern(a,b) extern a;
#endif

#include "bezier.h"


typedef struct route {
  bezier_curve3d milieu;
  bezier_curve1d inclinaison;
  double largeur;
} *route;

void repere_route(route r, double t, point3d centre, vecteur3d devant, vecteur3d cote, vecteur3d vertical);
void dessine_route(route r, int steps);

typedef struct position {
  /* position absolue de la voiture */
  point3d_cell centre;
  vecteur3d_cell devant;
  vecteur3d_cell dessus;
  /* temps de cette position */
  double last_update;
  /* position relative à la route, la coordonnée selon le vecteur D(index) est toujours nulle  */
  double index; // index de la courbe de bezier
  double y; // coordonnée selon le vecteur L(index) 
  double z; // coordonnée selon le vecteur N(index)
  /* vecteur vers l'avant exprimé dans le repère D(index), L(index), N(index) */
  vecteur3d_cell rel_devant;
  /* vecteur vers le haut de la voiture exprimé dans le repère D(index), L(index), N(index) */
  vecteur3d_cell rel_dessus;
  /* vecteur vitesse de la voiture en coordonnéé absolue */
  vecteur3d_cell vitesse;
  /* coefficient d'acceleration, de frottement linéaire et quadratique de la voiture (des constantes). */
  double coef_accel;
  double frot_lin;
  double frot_quad;
} *position;

void initTexture();
void initialize_position(route r, position p, double cur_time);
void update_position(route r, position p, double cur_time);
void set_camera_dessus(route r, position p);

// ratio de l'écran
extern(GLfloat ratio, 1.0);
// position du joystick/clavier
extern(double rotation, 0.0);
extern(double acceleration, 0.0);
// tableau avec deux routes (initialisé par initTexture()) 
extern(struct route routes[2], {});

extern(int debug, 0);


void dessine_voiture(position p);
#endif
