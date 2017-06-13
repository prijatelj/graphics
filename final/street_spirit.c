/*
 *  street_spirit.c
 *  A 3d and updated version of the 2d item drop game. Premise: You are in
 *  spirit form and are traveling through the streets attempting to activate
 *  spirit wards (protect the real world from the spirit world), destroy 
 *  nightmares (monsters causing trouble), and do so while trying to survive
 *  as long as you can. The game is randomly generated, w/ increasing difficulty
 *  as one progresses throughout the game.
 *  @author Derek S. Prijatelj
 *
 *  Objects in game:
 *  Player Sprite is white spirit ball with happy face (glusphere)
 *  Enemies are Red spirit balls with angry face    (glue sphere)
 *  HP+ item is green Teapots (glut) (after obtained a second of wire teapot)
 *  Spirit Wards are cyan Torus. (deactivated can be wire torus)
 *
 *  TODO
 *  - create basic Sphere world to be traveled
 *  - create Player Sprite
 *  - create Enemy Sprite
 *  - create HP Sprite
 *  - handle collisions and movement    (use cylinder hit "box")
 *  (base game done)
 *
 *  - Texture objects
 *  - texture world and create street lamps
 *  - create starry sky background (sky sphere)
 *
 *  - create change in probability for hp (dec) and enemies (inc)
 */

#include <glut/glut.h> // using mac, therefore uses this location
//#include <GL/glut.h> // Linux
// -lglut -lGL -lGLU -lm // Linux gcc flags

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// outside c files TODO make header files to link files
#include "lamp.c"

// Image Files
#include "img/sky.c"
#include "img/cobble.c"

#define WIN_WIDTH   800
#define WIN_HEIGHT  800
int win_width = WIN_WIDTH, win_height = WIN_HEIGHT;

#define LIGHT_NUM       47
#define KEY_ESC         27
#define KEY_W           119
#define KEY_A           97
#define KEY_S           115
#define KEY_D           100
#define WORLD_R         50
#define NUM_LAMPS       6

#define SPEED           5
#define VEL_RATE        5
#define VEL_MAX         50
#define ROT_MAX         3
#define FRICTION        .25
#define FRICTION_S      .8

#define PC_HP           5
#define PC_SIZE         2
#define PC_SPEED        2

#define ENEMY_NUM       10
#define ENEMY_HP        3
#define ENEMY_SIZE      2
#define ENEMY_SPAWN     40

#define TEAPOT_NUM      3
#define TEAPOT_HP       1
#define TEAPOT_SIZE     1.75
#define TEAPOT_SPAWN    1000

#define WARD_NUM        1
#define WARD_HP         1
#define WARD_SIZE       2
#define WARD_SPAWN      1000

time_t start = 0; // start time
void drawEnemies();
void drawItems();
void endGame();

// return random number between 0 and limit inclusive
int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;
    do { 
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

// Proper physics variables
float speed = SPEED;
float bounds = (WORLD_R*.55)/2;
float rotation = 0;
float rot_rate = 0;
double pc_y = (WORLD_R + PC_SIZE) * sin(35);
double pc_z = (WORLD_R + PC_SIZE) * cos(35);

// Score:
int hit = 0, dodged = 0, total = 0, heal = 0, ward_activated = 0;

// Objects:
int rand_lim(int);
int score = 0; // global score counter
typedef struct{
    int hp;     // hit points
    int state;  // state of sprite (0, ready to spawn, 1 on board, 2 dead)
    float x;    // location at bottom of screen
    float v;    // horizontal velocity
    float size; // radius of sphere
    float rot;  // how much the sprite has rotated w/ world, NPC only
    float loc;  // original location (rot angle of world) they spawned in 
} sprite;
sprite pc = {PC_HP, 1, 0, 0, PC_SIZE, 0, 0};
sprite enemies[ENEMY_NUM] = { 
    [0 ... ENEMY_NUM-1] = {ENEMY_HP, 0, 0, 0, ENEMY_SIZE, 0, 0}
};
sprite teapots[TEAPOT_NUM] = { 
    [0 ... TEAPOT_NUM-1] = {TEAPOT_HP, 0, 0, 0, TEAPOT_SIZE, 0, 0}
};
sprite wards[WARD_NUM] = { 
    [0 ... WARD_NUM-1] = {WARD_HP, 0, 0, 0, WARD_SIZE, 0, 0}
};

// Lights
GLfloat light_pos[] = { -100, 100.0, 100.0, 0.0 }; // x, y, z, direction
GLfloat light_amb[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat light_diff[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat light_spec[] = { 0.9, 0.9, 0.9, 1.0 };

GLfloat l1_pos[] = { 200, -150.0, 200.0, 1.0 };
GLfloat l1_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat l1_dif[] = { 0.25, 0.25, 0.25, 1.0 };
GLfloat l1_spc[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat l2_pos[] = { 0, -50.0, -100.0, 1.0 };
GLfloat l2_amb[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat l2_dif[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat l2_spc[] = { 1.0, 1.0, 1.0, 1.0 };

// Materials
GLfloat sky_dif[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat sky_spc[] = { 0.1, 0.1, 0.1, 1.0 };
GLfloat sky_shi[] = { 15 };
GLfloat sky_emi[] = { 1.0, 1.0, 1.0, 1.0 }; // may want it to be 100% visible

GLfloat streets_dif[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat streets_spc[] = { 0.05, 0.05, 0.05, 1.0 };
GLfloat streets_shi[] = { 50 };
GLfloat streets_emi[] = { 0, 0, 0, 1.0 };
//GLfloat streets_emi[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat sidewalk_dif[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat sidewalk_spc[] = { 0.05, 0.05, 0.05, 1.0 };
GLfloat sidewalk_shi[] = { 50 };
GLfloat sidewalk_emi[] = { 0, 0, 0, 1.0 };
//GLfloat sidewalk_emi[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat pc_dif[] = { 0.9, 0.9, 0.9, 0.55 };
GLfloat pc_spc[] = { 0.35, 0.15, 0.15, 0.5 };
GLfloat pc_shi[] = { 50.0 };
GLfloat pc_emi[] = { 0.3, 0.3, 0.3, 0.25 };

GLfloat enemy_dif[] = { 0.9, 0.2, 0.2, 0.55 };
GLfloat enemy_spc[] = { 0.4, 0.1, 0.1, 0.5 };
GLfloat enemy_shi[] = { 50.0 };
GLfloat enemy_emi[] = { 0.3, 0.1, 0.1, 0.25 };

GLfloat enemy2_dif[] = { 0.99, 0.2, 0.2, 0.55 };
GLfloat enemy2_spc[] = { 0.6, 0.1, 0.1, 0.5 };
GLfloat enemy2_shi[] = { 50.0 };
GLfloat enemy2_emi[] = { 0.6, 0.1, 0.1, 0.25 };

GLfloat teapot_dif[] = { 0.2, 0.9, 0.2, 0.55 };
GLfloat teapot_spc[] = { 0.1, 0.4, 0.1, 0.5 };
GLfloat teapot_shi[] = { 50.0 };
GLfloat teapot_emi[] = { 0.1, 0.3, 0.1, 0.25 };

GLfloat ward_dif[] = { 0.2, 0.5, 0.9, 0.55 };
GLfloat ward_spc[] = { 0.1, 0.2, 0.4, 0.5 };
GLfloat ward_shi[] = { 50.0 };
GLfloat ward_emi[] = { 0.1, 0.2, 0.3, 0.25 };

void skySphere(){
    //*
    glEnable(GL_TEXTURE_2D);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, sky.bytes_per_pixel,
        sky.width, sky.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        sky.pixel_data);
    //*/
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sky_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sky_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, sky_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, sky_emi);
    /*
    GLUquadricObj * sky_qobj = gluNewQuadric();
    gluQuadricNormals(sky_qobj, GLU_SMOOTH);
    gluQuadricTexture(sky_qobj, GLU_TRUE);
    gluSphere(sky_qobj, WORLD_R*10, 20, 20);
    gluDeleteQuadric(sky_qobj);
    */
    // Simple Rectangle Back Drop
    float r = WORLD_R*4;
    glPushMatrix();
    glTranslatef(0, WORLD_R*2.5, -WORLD_R*8);
    glBegin(GL_QUADS);
        glNormal3f(0, 0, 1); 
        glTexCoord2f(0, 0); glVertex3f(-r*.75, -r/4, 0); 
        glTexCoord2f(0, 1); glVertex3f(-r*.75, r/4, 0); 
        glTexCoord2f(1, 1); glVertex3f(r*.75, r/4, 0); 
        glTexCoord2f(1, 0); glVertex3f(r*.75, -r/4, 0); 
    glEnd();
    glPopMatrix();
    glDeleteTextures(1, &tex);
    glDisable(GL_TEXTURE_2D);
}

void sidewalk(){
    float radius = WORLD_R*1.01;
    //*
    glEnable(GL_TEXTURE_2D);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, cobble.bytes_per_pixel,
        cobble.width, cobble.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        cobble.pixel_data);
    //*/
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sidewalk_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, sidewalk_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, sidewalk_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, sidewalk_emi);
    
    GLUquadricObj * sidewalk_qobj = gluNewQuadric();
    gluQuadricNormals(sidewalk_qobj, GLU_SMOOTH);
    gluQuadricTexture(sidewalk_qobj, GLU_TRUE);
    gluCylinder(sidewalk_qobj, radius, radius, 10, 60, 60);
    gluDeleteQuadric(sidewalk_qobj);
    
    GLUquadricObj * sidewalk_cap_qobj = gluNewQuadric();
    gluQuadricNormals(sidewalk_cap_qobj, GLU_SMOOTH);
    gluQuadricTexture(sidewalk_cap_qobj, GLU_TRUE);
    gluDisk(sidewalk_cap_qobj, 0, radius, 60, 60);// maybe cyl?
    gluDeleteQuadric(sidewalk_cap_qobj);

    glDeleteTextures(1, &tex);
    glDisable(GL_TEXTURE_2D);
    
    glPushMatrix();
    glTranslatef(0, 0, 5); // center on sidewalk
    // loop for lamps
    int id_L = 1;
    for(float i = 0; i < 360; i += (360/NUM_LAMPS)){
        glPushMatrix();
        glRotatef(i, 0, 0, 1); // rotate evenly around z-axis
        glTranslatef(0, -radius, 0); // raise lamp to sidewalk radius
        lamp(id_L);
        glPopMatrix();
        id_L++;
    }
    glPopMatrix();
}
void streets(){
    float width = WORLD_R*.6;
    //*
    glEnable(GL_TEXTURE_2D);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, cobble.bytes_per_pixel,
        cobble.width, cobble.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        cobble.pixel_data);
    //*/
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, streets_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, streets_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, streets_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, streets_emi);
    //*
    glPushMatrix();
    glTranslatef(-width/2, 0 , 0);
    glRotatef(90, 0, 1, 0);
    GLUquadricObj * streets_qobj = gluNewQuadric();
    gluQuadricNormals(streets_qobj, GLU_SMOOTH);
    gluQuadricTexture(streets_qobj, GLU_TRUE);
    gluCylinder(streets_qobj, WORLD_R, WORLD_R, width, 60, 60);
    gluDeleteQuadric(streets_qobj);
    
    //Right Sidewalk
    glPushMatrix();
    glTranslatef(0, 0, width);
    sidewalk();
    glPopMatrix();
    //Left Sidewalk
    glPushMatrix();
    glRotatef(180, 1, 0, 0);
    glTranslatef(0, 0, 0);
    sidewalk();
    glPopMatrix();

    glPopMatrix();
    //*/
    glDeleteTextures(1, &tex);
    glDisable(GL_TEXTURE_2D);
}

void world(){
    // sky sphere
    skySphere(); // will not rotate, other than on y-axis turns
    
    // world base sphere
    if (rot_rate < ROT_MAX){
        rot_rate+= 0.1;
    } else if (rot_rate >= ROT_MAX){
        rot_rate = ROT_MAX;
    }
    rotation += rot_rate; 
    if (rotation >= 360){
        rotation = 0;
    }

    // For every change in rotation, the state 1 NPC sprites should update Rot

    glPushMatrix();
    glRotatef(rotation, 1, 0 , 0);
    streets();

    glPopMatrix();
    
    // Non-Player Sprites       ??? Proper location to draw them ???
    drawEnemies();
    drawItems();
}

void drawPlayer(){
    
    pc.v *= FRICTION;
    if (pc.v >= VEL_MAX){
        pc.v = VEL_MAX;
    } else if (pc.v <= -VEL_MAX){
        pc.v = -VEL_MAX;
    }
    pc.x += pc.v;
 
    // bounds
    if (pc.x >= bounds){
        pc.x = bounds;
        pc.v = 0;
    } else if (pc.x <= -bounds){
        pc.x = -bounds;
        pc.v = 0;
    }
    
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix(); 
    glRotatef(-35, 1, 0, 0); // rotate evenly around z-axis
    glTranslatef(pc.x, 0, WORLD_R + pc.size);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, pc_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, pc_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, pc_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, pc_emi); 
    
    if (pc.hp <= 0){
        pc.state = 2;
        glutWireSphere(pc.size, 60, 60);
        endGame();
    } else {
        GLUquadricObj * pc_qobj = gluNewQuadric();
        gluQuadricNormals(pc_qobj, GLU_SMOOTH);
        gluQuadricTexture(pc_qobj, GLU_TRUE);
        gluSphere(pc_qobj, pc.size, 60, 60);
        gluDeleteQuadric(pc_qobj);
    }
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

/*
 * 1 Dimensional check if within ranges on x-axis
 */
int left(char type, int i){
    float x;
    switch(type){
        case 'e':
             x = enemies[i].x - bounds;
            return x + enemies[i].size >= pc.x - pc.size &&
                    x - enemies[i].size <= pc.x - pc.size;
            break;
        case 't':
             x = teapots[i].x - bounds;
            return x + teapots[i].size >= pc.x - pc.size &&
                    x - teapots[i].size <= pc.x - pc.size;
            break;
        case 'w':
             x = wards[i].x - bounds;
            return x + wards[i].size >= pc.x - pc.size &&
                    x - wards[i].size <= pc.x - pc.size;
            break;
    }
    return 0;
}
int right(char type, int i){
    float x;
    switch(type){
        case 'e':
             x = enemies[i].x - bounds;
            return x - enemies[i].size <= pc.x + pc.size &&
                    x + enemies[i].size >= pc.x + pc.size;
            break;
        case 't':
             x = teapots[i].x - bounds;
            return x - teapots[i].size <= pc.x + pc.size &&
                    x + teapots[i].size >= pc.x + pc.size;
            break;
        case 'w':
             x = wards[i].x - bounds;
            return x - wards[i].size <= pc.x + pc.size &&
                    x + wards[i].size >= pc.x + pc.size;
            break;
    }
    return 0;
}
int p_in_n(char type, int i){
    float x;
    switch(type){
        case 'e':
             x = enemies[i].x - bounds;
            return (pc.x - pc.size <= x + enemies[i].size &&
                        x + enemies[i].size <= pc.x + pc.size) &&
                    (pc.x - pc.size <= x - enemies[i].size &&
                        x - enemies[i].size <= pc.x + pc.size);
            break;
        case 't':
             x = teapots[i].x - bounds;
            return (pc.x - pc.size <= x + teapots[i].size &&
                        x + teapots[i].size <= pc.x + pc.size) &&
                    (pc.x - pc.size <= x - teapots[i].size &&
                        x - teapots[i].size <= pc.x + pc.size);
            break;
        case 'w':
             x = wards[i].x - bounds;
            return (pc.x - pc.size <= x + wards[i].size &&
                        x + wards[i].size <= pc.x + pc.size) &&
                    (pc.x - pc.size <= x - wards[i].size &&
                        x - wards[i].size <= pc.x + pc.size);
            break;
    }
    return 0;
}
int n_in_p(char type, int i){
    float x;
    switch(type){
        case 'e':
             x = enemies[i].x - bounds;
            return (x - enemies[i].size <= pc.x + pc.size &&
                        pc.x + pc.size <= x + enemies[i].size) &&
                    (x - enemies[i].size <= pc.x - pc.size &&
                        pc.x - pc.size <= x + enemies[i].size);
            break;
        case 't':
             x = teapots[i].x - bounds;
            return (x - teapots[i].size <= pc.x + pc.size &&
                        pc.x + pc.size <= x + teapots[i].size) &&
                    (x - teapots[i].size <= pc.x - pc.size &&
                        pc.x - pc.size <= x + teapots[i].size);
            break;
        case 'w':
             x = wards[i].x - bounds;
            return (x - wards[i].size <= pc.x + pc.size &&
                        pc.x + pc.size <= x + wards[i].size) &&
                    (x - wards[i].size <= pc.x - pc.size &&
                        pc.x - pc.size <= x + wards[i].size);
            break;
    }
    return 0;
}
int intersectPC(char type, int i){
    int le = left(type, i);
    int ri = right(type, i);
    int pn = p_in_n(type, i);
    int np = n_in_p(type, i);
    return le || ri || pn || np;
}


void evilSpirit(int i){
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(-bounds, 0, 0);
    glRotatef(enemies[i].rot, 1, 0, 0);
    glTranslatef(enemies[i].x, 0, -(WORLD_R + enemies[i].size));
    glRotatef(rotation*(rand_lim(3)), 0, 0, (rand_lim(2)-1));
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, enemy_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, enemy_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, enemy_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, enemy_emi);

    if (enemies[i].state == 2){
        glutWireSphere(enemies[i].size, 4, 4);
    } else {
        GLUquadricObj * npc_qobj = gluNewQuadric();
        gluQuadricNormals(npc_qobj, GLU_SMOOTH);
        gluQuadricTexture(npc_qobj, GLU_TRUE);
        gluSphere(npc_qobj, enemies[i].size, 4, 4);
        gluDeleteQuadric(npc_qobj);
    }
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void teapotItem(int i){
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(-bounds, 0, 0);
    glRotatef(teapots[i].rot, 1, 0, 0);
    glTranslatef(teapots[i].x, 0, -(WORLD_R + teapots[i].size));
    glRotatef(-90, 1, 0 , 0);
    glRotatef(rotation*-2, 0, 1, 0);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, teapot_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, teapot_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, teapot_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, teapot_emi);
    
    if (teapots[i].state == 2){
        //glutWireSphere(teapots[i].size, 60, 60);
        glutWireTeapot(teapots[i].size);
    } else {
        /*
        GLUquadricObj * npc_qobj = gluNewQuadric();
        gluQuadricNormals(npc_qobj, GLU_SMOOTH);
        gluQuadricTexture(npc_qobj, GLU_TRUE);
        gluSphere(npc_qobj, enemies[i].size, 60, 60);
        gluDeleteQuadric(npc_qobj);
        */
        glutSolidTeapot(teapots[i].size);
    }
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void wardItem(int i){
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPushMatrix();
    glTranslatef(-bounds, 0, 0);
    glRotatef(wards[i].rot, 1, 0, 0);
    glTranslatef(wards[i].x, 0, -(WORLD_R + wards[i].size));
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ward_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ward_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, ward_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, ward_emi);
    
    if (wards[i].state == 2){
        //glutWireSphere(wards[i].size, 60, 60);
        glutWireTorus(wards[i].size/4, wards[i].size, 60, 60);
    } else {
        /*
        GLUquadricObj * npc_qobj = gluNewQuadric();
        gluQuadricNormals(npc_qobj, GLU_SMOOTH);
        gluQuadricTexture(npc_qobj, GLU_TRUE);
        gluSphere(npc_qobj, enemies[i].size, 60, 60);
        gluDeleteQuadric(npc_qobj);
        */
        glutSolidTorus(wards[i].size/4, wards[i].size, 60, 60);
    }
    glPopMatrix();

    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
}

void drawEnemies(){
    for (int i = 0; i < ENEMY_NUM; i++){
        switch(enemies[i].state){
            default:
            case 0: // randomly spawn
                if (rand_lim(ENEMY_SPAWN) == 0){ // 1 out of ENEMY_SPAWN times
                    enemies[i].x = rand_lim(bounds*2);
                    enemies[i].state = 1;
                    enemies[i].loc = rotation;// Need for knowing referenceAngle
                    evilSpirit(i);
                    total++;
                }
                break;
            case 1: // on board & in play
                // keep track of npc's rot
                enemies[i].rot += rot_rate;
                // if reached threshold, check collision
                // threshold = rot >= (180-40)
                if (enemies[i].rot >= 142 && enemies[i].rot < 153){
                    // Should check if sphere hit box intersects based on radii 
                    if (intersectPC('e', i)){
                        double e_y = (WORLD_R + enemies[i].size) * 
                                        sin(180 - enemies[i].rot);
                        double e_z = (WORLD_R + enemies[i].size) * 
                                        cos(180 - enemies[i].rot);
                        double dist = pow(pc.x-enemies[i].x, 2) + 
                                pow(pc_y - e_y, 2) + pow(pc_z - e_z, 2);
                        if (pow(pc.size + enemies[i].size, 2) <= dist){
                            enemies[i].state = 2;
                            pc.hp--;
                            puts("hit!");
                            hit++;
                        }
                    }
                } else if (enemies[i].rot >= 155){
                    dodged++;
                    enemies[i].state = 2;
                }
                evilSpirit(i);
                break;
            case 2: // hit and dead
                evilSpirit(i);
                enemies[i].state = 0;
                enemies[i].hp = ENEMY_HP;
                enemies[i].rot = 0;
                enemies[i].loc = 0;
                break;
        }
    
    }    
}

void drawTeapot(){
    for (int i = 0; i < TEAPOT_NUM; i++){
        switch(teapots[i].state){
            default:
            case 0: // randomly spawn
                if (rand_lim(TEAPOT_SPAWN) == 0){ // 1 out of TEAPOT_SPAWN times
                    teapots[i].x = rand_lim(bounds*2);
                    teapots[i].state = 1;
                    teapots[i].loc = rotation;// Need for knowing referenceAngle
                    teapotItem(i);
                    //total++;
                }
                break;
            case 1: // on board & in play
                // keep track of npc's rot
                teapots[i].rot += rot_rate;
                if (teapots[i].rot >= 142 && teapots[i].rot < 153){
                    // Should check if sphere hit box intersects based on radii 
                    if (intersectPC('t', i)){
                        double e_y = (WORLD_R + teapots[i].size) * 
                                        sin(180 - teapots[i].rot);
                        double e_z = (WORLD_R + teapots[i].size) * 
                                        cos(180 - teapots[i].rot);
                        double dist = pow(pc.x-teapots[i].x, 2) + 
                                pow(pc_y - e_y, 2) + pow(pc_z - e_z, 2);
                        if (pow(pc.size + teapots[i].size, 2) <= dist){
                            teapots[i].state = 2;
                            pc.hp++;
                            if (pc.hp > PC_HP){
                                pc.hp = PC_HP;
                            }
                            puts("Heal!");
                            heal++;
                        }
                    }
                } else if (teapots[i].rot >= 155){
                    //dodged++;
                    teapots[i].state = 2;
                }
                teapotItem(i);
                break;
            case 2: // hit and dead
                teapotItem(i);
                teapots[i].state = 0;
                teapots[i].hp = TEAPOT_HP;
                teapots[i].rot = 0;
                teapots[i].loc = 0;
                break;
        }
    }    
}

void drawWard(){
    for (int i = 0; i < WARD_NUM; i++){
        switch(wards[i].state){
            default:
            case 0: // randomly spawn
                if (rand_lim(WARD_SPAWN) == 0){ // 1 out of WARD_SPAWN times
                    wards[i].x = rand_lim(bounds*2);
                    wards[i].state = 1;
                    wards[i].loc = rotation;// Need for knowing referenceAngle
                    wardItem(i);
                    //total++;
                }
                break;
            case 1: // on board & in play
                // keep track of npc's rot
                wards[i].rot += rot_rate;
                if (wards[i].rot >= 142 && wards[i].rot < 153){
                    // Should check if sphere hit box intersects based on radii 
                    if (intersectPC('w', i)){
                        double e_y = (WORLD_R + wards[i].size) * 
                                        sin(180 - wards[i].rot);
                        double e_z = (WORLD_R + wards[i].size) * 
                                        cos(180 - wards[i].rot);
                        double dist = pow(pc.x-wards[i].x, 2) + 
                                pow(pc_y - e_y, 2) + pow(pc_z - e_z, 2);
                        if (pow(pc.size + wards[i].size, 2) <= dist){
                            wards[i].state = 2;
                            puts("Spirit Ward Activated!");
                            ward_activated++;
                        }
                    }
                } else if (wards[i].rot >= 155){
                    wards[i].state = 2;
                }
                wardItem(i);
                break;
            case 2: // hit and dead
                wardItem(i);
                wards[i].state = 0;
                wards[i].hp = WARD_HP;
                wards[i].rot = 0;
                wards[i].loc = 0;
                break;
        }
    }    
}

void drawItems(){
    drawTeapot();
    drawWard();
}

void scene(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    // set up environment and Non-Player Sprites
    world();
    
    drawPlayer();

    glPopMatrix();
}

void endGame(){
    puts("\nGame Results:");
    if (pc.state == 0 || pc.state == 2)
        puts("You Died.");
    else
        puts("You Quit.");
    printf("You survived for %.0f seconds\n\n", difftime(time(0), start));
    printf("Total Enemies Dodged: %d\n", dodged);
    printf("Total Enemy Collisions: %d\n", hit);
    printf("Total Evil Spirits: %d\n", total);
    printf("Spirit Green Tea Drunk: %d pots\n", heal);
    printf("Spirit Wards Activated: %d\n\n", ward_activated);
    exit(0);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
    glFlush();
}

void reshape(GLsizei w, GLsizei h){
    glutReshapeWindow(win_width,win_height);
}

void idle(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
    glFlush();
}

void keyInput(unsigned char key, int x, int y){
    switch(key){
        case KEY_ESC:   // Quit Application
            endGame();
            break;
        case KEY_W:
            glMatrixMode(GL_PROJECTION);
            glRotatef(speed, 1, 0, 0);
            break;
        case KEY_S:
            glMatrixMode(GL_PROJECTION);
            glRotatef(-speed, 1, 0, 0);
            break;
        case KEY_A:
            glMatrixMode(GL_PROJECTION);
            glRotatef(speed, 0, 1, 0);
            break;
        case KEY_D:
            glMatrixMode(GL_PROJECTION);
            glRotatef(-speed, 0, 1, 0);
            break;
    }
}

void keySpecial(int key, int x, int y){
   
    // WORLD_R*.6 = bounds
    
    switch(key){
        // around y axis
        case GLUT_KEY_LEFT:  // rotate left neg-x of camera
            //if (pc.v > -PC_SPEED){
            if (pc.v > 0){
                pc.v = -VEL_RATE;
            } else {
                pc.v-= VEL_RATE;
            }
            //}
            break;
        case GLUT_KEY_RIGHT: // rotate right pos-x of camera
            //if (pc.v < PC_SPEED){
            if (pc.v < 0) {
                pc.v = VEL_RATE;
            } else {
                pc.v+= VEL_RATE;
            }
            //}
            break;
        // around x axis
        case GLUT_KEY_UP: // rotate up pos-y of camera
            break;
        case GLUT_KEY_DOWN: // rotate down neg-y of camera
            break;
        default:
            break;
    }
}

void initGL(){
    srand(time(NULL));// initialize random.

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(30, 1.0f, 20, 2000);
    //glTranslatef(0, 0, -WORLD_R*6); // VIEW WORK
    glTranslatef(0, -WORLD_R/1.2, -WORLD_R*2); // actual in game.
    //glRotatef(45, 1, 1, 0);
    glPushMatrix(); // save first view

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_amb);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

int main(int argc, char **argv){
    if (GL_MAX_LIGHTS < LIGHT_NUM){
        printf("GL_MAX_LIGHTS = %d\n", GL_MAX_LIGHTS);
        puts("Error: System's max number of lights is less than the required number of lights");
        return(-1);
    }
    start = time(0);// strt timer.

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Street Spirit ver.Alpha_0.1");

    initGL();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keySpecial);
    glutIdleFunc(idle);
    glutMainLoop();
    return(0);
}
