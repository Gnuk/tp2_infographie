#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <SDL/SDL_image.h>
#include <math.h>
#include <stdio.h>
#include "bezier.h"
#define extern(a,b) a = b;
#include "route.h"

struct bezier_curve3d milieu_route1 = {
  .nb_points = 9,
  .controles = { 
    {.x = 0.0, .y = 0.0, .z = 0.0 },
    {.x = 0.0, .y = 100.0, .z = 0.0 },
    {.x = 50.0, .y = 150.0, .z = 20.0 },
    {.x = 100.0, .y = 100.0, .z = 0.0 },
    {.x = 100.0, .y = 0.0, .z = 0.0 },
    {.x = 100.0, .y = -100.0, .z = 0.0 },
    {.x = 50.0, .y = -150.0, .z = 20.0 },
    {.x = 0.0, .y = -100.0, .z = 0.0 },
    {.x = 0.0, .y = 0.0, .z = 0.0 },
  }
};

struct bezier_curve1d inclinaison_route1 = {
  .nb_points = 9,
  .controles = { 0.0, 0.0, -0.7, 0.0, 0.0, 0.0, -0.7, 0.0, 0.0 } 
};

struct route route1 = {
  .milieu = &milieu_route1,
  .inclinaison = &inclinaison_route1,
  .largeur = 2.0
};

struct bezier_curve3d milieu_route2 = {
  .nb_points = 13,
  .controles = { 
    {.x = 0.0, .y = 0.0, .z = 0.0 },
    {.x = 100.0, .y = 50.0, .z = 0.0 },
    {.x = 100.0, .y = 100.0, .z = 0.0 },
    {.x = 0.0, .y = 150.0, .z = 20.0 },
    {.x = -100.0, .y = 100.0, .z = 0.0 },
    {.x = -100.0, .y = 50.0, .z = 0.0 },
    {.x = 0.0, .y = 0.0, .z = 20.0 },
    {.x = 100.0, .y = -50.0, .z = 0.0 },
    {.x = 100.0, .y = -100.0, .z = 0.0 },
    {.x = 0.0, .y = -150.0, .z = 20.0 },
    {.x = -100.0, .y = -100.0, .z = 0.0 },
    {.x = -100.0, .y = -50.0, .z = 0.0 },
    {.x = 0.0, .y = 0.0, .z = 0.0 },
  }
};

struct bezier_curve1d inclinaison_route2 = {
  .nb_points = 13,
  .controles = { 0.0, 0.0, 0.0, 0.7, 0.0, 0.0, 0.0, 0.0, 0.0, -0.7, 0.0, 0.0, 0.0 } 
};

struct route route2 = {
  .milieu = &milieu_route2,
  .inclinaison = &inclinaison_route2,
  .largeur = 2.0
};



static vecteur3d_cell vert0 = { .x = 0.0, .y = 0.0, .z = 1.0 };
  
void repere_route(route r, double t, point3d centre, vecteur3d devant, vecteur3d cote, vecteur3d vertical) {
  point3d_cell centre_suivant;
  calcule3d(centre, r->milieu, t);
  calcule3d(&centre_suivant, r->milieu, t+1e-6);
  double inclinaison = calcule1d(r->inclinaison, t);
  vec3d(devant, centre, &centre_suivant);
  normalize3d(devant);
  *vertical = vert0;
  vec_prod3d(cote, vertical, devant);
  normalize3d(cote);
  scal_prod3d(cote, inclinaison);
  add3d(vertical, cote);
  normalize3d(vertical);
  vec_prod3d(cote, devant, vertical);
  normalize3d(cote);
}

GLuint texture_route;
double texture_ratio = 1.0;

void initTexture() {
  SDL_Surface * texture;
  // ---- Création d'un objet de texture. ------------------------------------------------
  glGenTextures (1, & texture_route);
  glBindTexture (GL_TEXTURE_2D, texture_route);
  // Paramétrage de la texture.
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // Chargement du fichier.
  texture = IMG_Load ("route.jpg" );
   if(!texture) {
      printf("IMG_Load: %s\n", IMG_GetError());
      exit(1);
    }
  // Jonction entre OpenGL et SDL.
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB8, texture->w, texture->h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, texture-> pixels);
  texture_ratio = (double) texture->h / (double) texture->w;

  routes[0] = route1;
  routes[1] = route2;
}

void dessine_route(route r, int steps) {
  int i, start = 1;
  double tex_x = 0.0;
  point3d_cell previous_centre;
  vecteur3d_cell ecart_centre;
  glEnable (GL_TEXTURE_2D);
  glBindTexture (GL_TEXTURE_2D, texture_route);
  glColor3f(   1.0f,  1.0f,  1.0f ); 
  glBegin(GL_QUAD_STRIP);
  for(i=0;i<=steps;i++) {
    double t = (float) i / (float) steps;
    point3d_cell centre,x1,x2;
    vecteur3d_cell devant, cote, vertical;
    repere_route(r, t, &centre, &devant, &cote, &vertical);
    if (start) {previous_centre = centre; start = 0;};
    vec3d(&ecart_centre, &previous_centre, &centre);
    tex_x = tex_x + norm3d(&ecart_centre)/(texture_ratio*2*r->largeur);
    glNormal3dv((double*) &vertical);
    x1 =centre; x2 = centre;
    translate3d(&x1, r->largeur, &cote);
    translate3d(&x2, -r->largeur, &cote);
    glTexCoord2f(0.0, tex_x);
    glVertex3dv((double*) &x1);
    glTexCoord2f(1.0, tex_x);
    glVertex3dv((double*) &x2);
    previous_centre = centre;
  }
  glEnd();
  glDisable (GL_TEXTURE_2D);
}

/* Mise à jour de la position de la voiture */

void update_position(route r, position p, double cur_time) {
  point3d_cell centre;
  vecteur3d_cell devant, cote, dessus;

  // calcul du repère à l'index t = p->index
  repere_route(r, p->index, &centre, &devant, &cote, &dessus);

  point3d_cell centre_d;
  vecteur3d_cell devant_d, cote_d, dessus_d;
  
  // et ç l'index t + dt. 
  repere_route(r, p->index+1e-6, &centre_d, &devant_d, &cote_d, &dessus_d);

  if (debug) fprintf(stderr, "centre = (%lf, %lf, %lf)\n", centre.x, centre.y , centre.z);
  if (debug) fprintf(stderr, "devant = (%lf, %lf, %lf)\n", devant.x, devant.y , devant.z);
  if (debug) fprintf(stderr, "cote = (%lf, %lf, %lf)\n", cote.x, cote.y , cote.z);
  if (debug) fprintf(stderr, "dessus = (%lf, %lf, %lf)\n", dessus.x, dessus.y , dessus.z);

  // cacule du gradient de P(t) = P'(t) 
  vecteur3d_cell d_centre;
  vec3d(&d_centre,&centre,&centre_d);
  scal_prod3d(&d_centre, 1e6);

  // intervalle de temps écoulé et mise à jour de p->lastime.
  double delta_t = cur_time - p->last_update;
  p->last_update = cur_time;

  if (debug) fprintf(stderr,"delta = %lf\n", delta_t);

  /* Rotation : on a directement la vitesse de rotation dans la variable globale rotation */
  double alpha = rotation * delta_t;
  
  // on fait cette rotation en modifiant les vecteur p->rel_devant et p->rel_dessus 
  // on aurait pu faire avec p->devant et p->dessus
  vecteur3d_cell rel_cote;
  vec_prod3d(&rel_cote, &(p->rel_dessus), &(p->rel_devant));
  scal_prod3d(&p->rel_devant, cos(alpha));
  translate3d((point3d) &(p->rel_devant), sin(alpha), &rel_cote);
  normalize3d(&(p->rel_devant));

  if (debug) fprintf(stderr, "p->rel_devant = (%lf, %lf, %lf)\n", p->rel_devant.x, p->rel_devant.y , p->rel_devant.z);

  // on calcule les coordonnées absolue de p->devant et p->dessus à partir 
  // des coordonnées relatives à la route. Elle ne sont pas mie à jour plus tôt.
  zero3d(&(p->devant));
  translate3d((point3d) &(p->devant), p->rel_devant.x, &devant);
  translate3d((point3d) &(p->devant), p->rel_devant.y, &cote);
  translate3d((point3d) &(p->devant), p->rel_devant.z, &dessus);
  normalize3d(&(p->devant));

  if (debug) fprintf(stderr, "p->devant = (%lf, %lf, %lf)\n", p->devant.x, p->devant.y , p->devant.z);

  zero3d(&(p->dessus));
  translate3d((point3d) &(p->dessus), p->rel_dessus.x, &devant);
  translate3d((point3d) &(p->dessus), p->rel_dessus.y, &cote);
  translate3d((point3d) &(p->dessus), p->rel_dessus.z, &dessus);
  normalize3d(&(p->dessus));

  if (debug) fprintf(stderr, "p->dessus = (%lf, %lf, %lf)\n", p->dessus.x, p->dessus.y , p->dessus.z);

  /* Mise à jour vitesse, après calcul de l'accélération */
  // initialization avec le force de gravité.
  vecteur3d_cell accel = { .x = 0.0, .y = 0.0, .z = -9.81 };
  if (debug) fprintf(stderr,"acceleration = %lf\n", acceleration);
  if (acceleration > 0.0) if (debug) fprintf(stderr, "  non zero\n");
  // acceleration du à l'accelerateur (dans la variable globale accelerateur 
  translate3d((point3d) &accel, p->coef_accel*acceleration, &(p->devant));
  if (p->z <= 0.0) {
    //si on touche la route
    //calcul des forttements
    double coef2 = fabs(p->y)>r->largeur?r->largeur-fabs(p->y)-p->frot_quad:-p->frot_quad;
    translate3d((point3d) &accel, coef2*norm3d(&(p->vitesse))-p->frot_lin, &(p->vitesse));
    //projecttion de la vitesse et de l'acceleration pour qu'elles soient parallèle à la route.
    double t = dot_prod3d(&accel, &dessus);
    translate3d((point3d) &accel, -t, &dessus);
    t = dot_prod3d(&(p->vitesse), &dessus);
    translate3d((point3d) &accel, -t, &dessus);
  }
  // on peut enfin mettre à jour la vitesse
  if (debug) fprintf(stderr, "accel = (%lf, %lf, %lf)\n", accel.x, accel.y , accel.z);
  translate3d((point3d) &(p->vitesse), delta_t, &accel);
  p->z = 0.0;

  if (debug) fprintf(stderr, "p->vitesse = (%lf, %lf, %lf)\n", p->vitesse.x, p->vitesse.y , p->vitesse.z);

  // on met maintenant à jour les coordonnées relative. Attention pour l'index,
  // cela dépend de la norme de P'(t) = d_centre.
  double d_index = dot_prod3d(&d_centre, &(p->vitesse)) / norm2_3d(&d_centre);
  double d_y = dot_prod3d(&cote, &(p->vitesse));
  double d_z = dot_prod3d(&dessus, &(p->vitesse));
  p->index += d_index * delta_t;
  // l'index doit rester entre 0 et 1 !
  while (p->index >= 1.0) p->index -= 1.0;
  while (p->index < 0.0) p->index += 1.0;
  p->y += d_y * delta_t;
  p->z += d_z * delta_t;

  if (debug) fprintf(stderr, "d_index = %lf, d_y = %lf, d_z = %lf\n", d_index, d_y, d_z);

  /* calcul des nouvelles coordonnées absolues */

  p->centre = centre;
  translate3d(&(p->centre), p->y, &cote);
  translate3d(&(p->centre), p->z, &dessus);

  if (debug) fprintf(stderr, "p->centre = (%lf, %lf, %lf)\n", p->centre.x, p->centre.y , p->centre.z);
  
  /* calcule des coordonnées relative dans le nouveau reprère, on vient de bouger */
  repere_route(r, p->index, &centre, &devant, &cote, &dessus);
  
  // on calcule d'abord le déplacement. 
  vecteur3d_cell delta_centre;
  vec3d(&delta_centre, &centre, &(p->centre));

  // x ci dessous doit rester petit, mais on fait une simple approximation linéaire et il ne reste pas
  // si petit que ça.
  double x = dot_prod3d(&delta_centre, &devant); 
  p->y = dot_prod3d(&delta_centre, &cote);
  p->z = dot_prod3d(&delta_centre, &dessus);

  if (debug) fprintf(stderr, "x = %lf, p->y = %lf; p->z = %lf\n", x, p->y, p->z);

  // mise à jour du vecteur p->rel_devant en fonction du nouveau repère.
  p->rel_devant.x = dot_prod3d(&devant, &(p->devant));
  p->rel_devant.y = dot_prod3d(&cote, &(p->devant));
  p->rel_devant.z = dot_prod3d(&dessus, &(p->devant));
  normalize3d(&(p->rel_devant));

  /* on garde la caméra parallèle à la route,
     sinon ça saute. Dommage, ce n'est pas très realiste
     en cas de saut !, mais il faudrait sinon mettre des suspensions et 
     tenir compte des points de contact, et du moment d'intertie de la voiture */
  p->rel_dessus.x = 0.0;
  p->rel_dessus.y = 0.0;
  p->rel_dessus.z = 1.0;
  p->rel_devant.z *= 0.0;
  normalize3d(&(p->rel_devant));

  if (debug) fprintf(stderr, "p->rel_devant = (%lf, %lf, %lf)\n", p->rel_devant.x, p->rel_devant.y , p->rel_devant.z);
  if (debug) fprintf(stderr, "p->rel_dessus = (%lf, %lf, %lf)\n\n\n", p->rel_dessus.x, p->rel_dessus.y , p->rel_dessus.z);
}

void initialize_position(route r, position p, double curtime) {
  p->index = 0.0;
  p->y = 0.0;
  p->z = 0.0;
  p->last_update = curtime;
  zero3d(&(p->rel_devant));
  p->rel_devant.x = 1.0;
  zero3d(&(p->rel_dessus));
  p->rel_dessus.z = 1.0;

  p->coef_accel = 30.0;
  p->frot_quad = 0.03;
  p->frot_lin = 0.5;

  /* calcule des autres champs, on utilise le fait que acceleration = rotation = 0.0 
     au départ */
  update_position(r, p, curtime);
}


void set_camera_dessus(route r, position p) {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.5f, 3000.0f );

    point3d_cell camera = p->centre;
    translate3d(&camera, r->largeur, &vert0);
    translate3d(&camera, -3.0*r->largeur, &(p->devant));

    point3d_cell centre = camera;
    translate3d(&centre, 3.0*r->largeur, &(p->devant));

    gluLookAt(camera.x, camera.y, camera.z, centre.x, centre.y, centre.z, 0.0, 0.0, 1.0);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );
}

void dessine_voiture(position p) {
  point3d_cell p1 = p->centre, p2 = p->centre, p3 = p->centre, p4 = p->centre;
  vecteur3d_cell cote;
  vec_prod3d(&cote,&(p->dessus),&(p->devant));
  vecteur3d_cell opp_cote = cote;
  vecteur3d_cell derriere = p->devant;
  scal_prod3d(&opp_cote, -1);
  scal_prod3d(&derriere, -1);

  translate3d(&p1, 1.0, &cote);
  translate3d(&p2, -1.0, &cote);
  translate3d(&p3, 0.5, &(p->dessus));
  translate3d(&p4, 4.0, &(p->devant));

  glColor3f(   0.7f,  0.3f,  0.2f ); 
  glBegin(GL_TRIANGLES);
  glNormal3dv((double*) &derriere);
  glVertex3dv((double*) &p1);
  glVertex3dv((double*) &p2);
  glVertex3dv((double*) &p3);
  
  glNormal3dv((double*) &cote);
  glVertex3dv((double*) &p1);
  glNormal3dv((double*) &p->dessus);
  glVertex3dv((double*) &p3);
  glNormal3dv((double*) &p->devant);
  glVertex3dv((double*) &p4);

  glNormal3dv((double*) &opp_cote);
  glVertex3dv((double*) &p2);
  glNormal3dv((double*) &p->devant);
  glVertex3dv((double*) &p4);
  glNormal3dv((double*) &p->dessus);
  glVertex3dv((double*) &p3);
  
  glEnd();
}
