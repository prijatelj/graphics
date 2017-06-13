/*
 *  human.c - animated human character in opengl.
 *  @author Derek S. Prijatelj
 */

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h> // using mac, therefore uses this location
#include <math.h>
#include <time.h>
#include "face.c"
#include "jaw.c"
#include "flesh.c"
//#include "plaid.c"
//#include "jeans.c"
#include "chin.c"
#include "nose.c"

#define SPEED       5
#define KEY_ESC     27
float vx = 0, vy = 0, vz = 0;
float speed = SPEED;

#define WIN_WIDTH   800
#define WIN_HEIGHT  800
int win_width = WIN_WIDTH, win_height = WIN_HEIGHT;
#define MAIN_HEIGHT 80
#define TABLE_DIM   32
#define ORANGE_R 5

GLfloat light_pos[] = { -100, 100.0, 100.0, 1.0 }; // x, y, z, direction
GLfloat light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diff[] = { 0.95, 0.95, 0.95, 1.0 };
GLfloat light_spec[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat l1g_pos[] = { 200, -150.0, 200.0, 1.0 };
GLfloat l1g_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat l1g_diff[] = { 0.25, 0.25, 0.25, 1.0 };
GLfloat l1g_spec[] = { 0.5, 0.5, 0.5, 1.0 };

GLfloat l2_pos[] = { 0, -50.0, -100.0, 1.0 };
GLfloat l2_amb[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat l2_diff[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat l2_spec[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat s[] = { 0.3, 0.1, 0.1, 1.0 };
GLfloat s_amb[] = { 0.3, 0.1, 0.1, 1.0 };
GLfloat s_spc[] = { 0.35, 0.15, 0.15, 1.0 };
GLfloat s_shi[] = { 2.0 };

GLfloat p[] = { 0.1, 0.1, 0.2, 1.0 };
GLfloat p_amb[] = { 0.1, 0.1, 0.2, 1.0 };
GLfloat p_spc[] = { 0.15, 0.15, 0.25, 1.0 };
GLfloat p_shi[] = { 2.0 };

GLfloat shoe[] = { 0.15, 0.1, 0.1, 1.0 };
GLfloat shoe_amb[] = { 0.15, 0.1, 0.1, 1.0 };
GLfloat shoe_spc[] = { 0.15, 0.1, 0.1, 1.0 };
GLfloat shoe_shi[] = { 2.0 };

GLfloat skin[] = { 0.35, 0.25, 0.23, 0.5 };
GLfloat skin_amb[] = { 0.35, 0.25, 0.23, 1.0 };
GLfloat skin_spc[] = { 0.45, 0.34, 0.23, 1.0 };
GLfloat skin_shi[] = { 2.0 };

float shrug_height = 0;
float shrug_rotate = 0;
int shrug_up = 1; // 0 = false
int pause = 0; 
int pause_time = 0;

/*
 equilateralPolygon()
    Makes an equilateral polygon with the given amount of edges and at the
    specified rotation. the rotation specifies what degree angle to make
    first point. Draws counter clockwise.

    plane:
    0   xy
    1   xz
    2   yz
*/
void equilateralPolygon(float r, int edges, float rotation,
    float x_offset, float y_offset, float x_mul, float y_mul,
    int plane, float z_offset){
    glBegin(GL_POLYGON);
    float theta;
    if (plane == 0){ 
        for(theta = M_PI*rotation/180.0f;
            theta <= M_PI*(rotation+360.0f)/180.0f;
            theta +=2*M_PI/edges){
            glVertex3f((x_mul*r*cos(theta))+x_offset,
                (y_mul*r*sin(theta))+y_offset, z_offset);
        }   
    }   
    else if (plane == 1){ 
        for(theta = M_PI*rotation/180.0f;
            theta <= M_PI*(rotation+360.0f)/180.0f;
            theta +=2*M_PI/edges){
            glVertex3f((x_mul*r*cos(theta))+x_offset, z_offset,
                (y_mul*r*sin(theta))+y_offset);
        }   
    }   
    else{
        for(theta = M_PI*rotation/180.0f;
            theta <= M_PI*(rotation+360.0f)/180.0f;
            theta +=2*M_PI/edges){
            glVertex3f(z_offset, (y_mul*r*sin(theta))+y_offset,
                (x_mul*r*cos(theta))+x_offset);
        }   
    }   
    glEnd();
}

void head(){
    float head_w = 3, lower_jaw = head_w/1.2, lower_jaw_l = 2.5;
    glPushMatrix();
    glTranslatef(0, -0.5, 0);
    glRotatef(90, 1, 0, 0);
    glRotatef(180, 0, 0, 1);
    //glRotatef(10, 1, 0, 0);
    glScalef(.9, 1, 1);
    //glScalef(1, .8, 1);
    //glScalef(.8, 1.2, 1);
   
    glPushMatrix();
    //glScalef(1, 1, .7);
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, face.bytes_per_pixel,
        face.width, face.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        face.pixel_data);

    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricDrawStyle(qobj, GLU_FILL);
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricTexture(qobj, GLU_TRUE); // Texture Coords On
    //sphere = glGenLists(1);
    //glNewList(sphere, GL_COMPILE);
    gluSphere(qobj, head_w, 20, 20);
    //glEndList();
    gluDeleteQuadric(qobj);
    glDeleteTextures(1, &tex);
    glPopMatrix();

    glPushMatrix();// nose
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, nose.bytes_per_pixel,
        nose.width, nose.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        nose.pixel_data);

    glTranslatef(0.15, -2.8, .75);
    glScalef(1, 1, 1.3);
    GLUquadricObj * nose_qobj = gluNewQuadric();
    gluQuadricDrawStyle(nose_qobj, GLU_FILL);
    gluQuadricNormals(nose_qobj, GLU_SMOOTH);
    gluQuadricTexture(nose_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(nose_qobj, head_w/5, 20, 20);
    gluDeleteQuadric(nose_qobj);
    glDeleteTextures(1, &tex);
    glPopMatrix();


    //GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, jaw.bytes_per_pixel,
        jaw.width, jaw.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        jaw.pixel_data);
    
    glTranslatef(0, -1.4, 0);
    glScalef(1, .5, 1);

    GLUquadricObj * jaw_obj = gluNewQuadric();
    gluQuadricDrawStyle(jaw_obj, GLU_FILL);
    gluQuadricNormals(jaw_obj, GLU_SMOOTH);
    gluQuadricTexture(jaw_obj, GLU_TRUE); // Texture Coords On
    gluCylinder(jaw_obj, lower_jaw, lower_jaw, lower_jaw_l, 20, 20);
    gluDeleteQuadric(jaw_obj);
    glDeleteTextures(1, &tex);


    glTranslatef(0, 0, lower_jaw_l);
    glScalef(1, 1, .25);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, chin.bytes_per_pixel,
        chin.width, chin.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        chin.pixel_data);

    GLUquadricObj * chin_qobj = gluNewQuadric();
    gluQuadricDrawStyle(chin_qobj, GLU_FILL);
    gluQuadricNormals(chin_qobj, GLU_SMOOTH);
    gluQuadricTexture(chin_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(chin_qobj, lower_jaw, 20, 20);
    gluDeleteQuadric(chin_qobj);
    glDeleteTextures(1, &tex);

    glPopMatrix();
}
void neck(){
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, flesh.bytes_per_pixel,
        flesh.width, flesh.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        flesh.pixel_data);
    
    glPushMatrix();
    glTranslatef(0, 13.5, 0);
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluQuadricTexture(qobj, GLU_TRUE); // Texture Coords On
    gluCylinder(qobj, 1.8, 1.8, 6, 20, 20);
    gluDeleteQuadric(qobj);
    glDeleteTextures(1, &tex);
    glPopMatrix();
    head();
    glPopMatrix();
}

void hand(float wrist_w){
    float palm_w = wrist_w/1.25, palm_l = 1;
    float j1_w = .25, f1_l = .25;
    
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, flesh.bytes_per_pixel,
        flesh.width, flesh.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        flesh.pixel_data);
    
    glTranslatef(.25, 0, 0); 
    glScalef(.8, 1, 1); 
    //glRotatef(-90, .5, 1, 0);
    GLUquadricObj * hand_qobj = gluNewQuadric();
    gluQuadricNormals(hand_qobj, GLU_SMOOTH);
    gluQuadricTexture(hand_qobj, GLU_TRUE); // Texture Coords On
    gluCylinder(hand_qobj, wrist_w, palm_w, palm_l, 20, 20);
    gluDeleteQuadric(hand_qobj);
    
    glTranslatef(0, 0, palm_l);
    GLUquadricObj * palm_qobj = gluNewQuadric();
    gluQuadricNormals(palm_qobj, GLU_SMOOTH);
    gluQuadricTexture(palm_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(palm_qobj, palm_w, 20, 20);
    gluDeleteQuadric(palm_qobj);

    //glTranslatef(0, 0, palm_l);
    glRotatef(-45, 0, 1, 0);
    glScalef(.98, 1, 1);
    GLUquadricObj * hand2_qobj = gluNewQuadric();
    gluQuadricNormals(hand2_qobj, GLU_SMOOTH);
    gluQuadricTexture(hand2_qobj, GLU_TRUE); // Texture Coords On
    gluCylinder(hand2_qobj, palm_w, palm_w, palm_l/1.5, 20, 20);
    gluDeleteQuadric(hand2_qobj);
    
    glTranslatef(0, 0, palm_l/1.5);
    GLUquadricObj * palm2_qobj = gluNewQuadric();
    gluQuadricNormals(palm2_qobj, GLU_SMOOTH);
    gluQuadricTexture(palm2_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(palm2_qobj, palm_w, 20, 20);
    gluDeleteQuadric(palm2_qobj);


    //glTranslatef(0, 0, palm_l);
    glRotatef(-15, 0, 1, 0);
    glScalef(.98, 1, 1);
    GLUquadricObj * hand3_qobj = gluNewQuadric();
    gluQuadricNormals(hand3_qobj, GLU_SMOOTH);
    gluQuadricTexture(hand3_qobj, GLU_TRUE); // Texture Coords On
    gluCylinder(hand3_qobj, palm_w, palm_w, palm_l/1.8, 20, 20);
    gluDeleteQuadric(hand3_qobj);
    
    glTranslatef(0, 0, palm_l/1.8);
    GLUquadricObj * palm3_qobj = gluNewQuadric();
    gluQuadricNormals(palm3_qobj, GLU_SMOOTH);
    gluQuadricTexture(palm3_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(palm3_qobj, palm_w, 20, 20);
    gluDeleteQuadric(palm3_qobj);

    /*
    for(int i = 0; i < 4; i++){
        glTranslatef(0, i, 0);

        GLUquadricObj * f1_qobj = gluNewQuadric();
        gluQuadricNormals(f1_qobj, GLU_SMOOTH);
        gluQuadricTexture(f1_qobj, GLU_TRUE); // Texture Coords On
        gluCylinder(f1_qobj, j1_w, j1_w, f1_l, 20, 20);
        gluDeleteQuadric(f1_qobj);

        glTranslatef(0, 0, f1_l);
        GLUquadricObj * j1_qobj = gluNewQuadric();
        gluQuadricNormals(j1_qobj, GLU_SMOOTH);
        gluQuadricTexture(j1_qobj, GLU_TRUE); // Texture Coords On
        gluSphere(j1_qobj, j1_w, 20, 20);
        gluDeleteQuadric(j1_qobj);
    }
    */

    glDeleteTextures(1, &tex);

}

/*
 * Should make him shrug! Simple and Easy animation, better than waving.
 */
void arm(char orient){
    float shldr_w = 2.5, elbow_w = 1.75, wrist_w = 1, neck_w = 2;
    float upperarm_l = 5, forearm_l = 7, traps_l = 5;
    float elbow_angle;
    if (orient == 'l'){
        elbow_angle = -30;
    } else{
        elbow_angle = 30;
    }
    GLUquadricObj * shoulder = gluNewQuadric();
    gluQuadricNormals(shoulder, GLU_SMOOTH);
    gluSphere(shoulder, shldr_w, 20, 20);
    gluDeleteQuadric(shoulder);
    
    // Traps
    glPushMatrix();
    glRotatef(-90, .5, 1, 0);
    GLUquadricObj * traps_qobj = gluNewQuadric();
    gluQuadricNormals(traps_qobj, GLU_SMOOTH);
    gluCylinder(traps_qobj, shldr_w, neck_w, traps_l, 20, 20);
    gluDeleteQuadric(traps_qobj);
    glPopMatrix();    
    
    //  Needs to increase angle of upper to torso when shrugging

    // Upper Arm
    glPushMatrix();
    if (orient == 'l'){
        glRotatef(105, 1, 0, 0);
    } else {
        glRotatef(75, 1, 0, 0);
    }
    glRotatef(25 + shrug_height*65, 0, 1, 0);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluCylinder(qobj, shldr_w, elbow_w, upperarm_l, 20, 20);
    gluDeleteQuadric(qobj);

    // elbow
    glTranslatef(0, 0, upperarm_l);

    GLUquadricObj * elbow = gluNewQuadric();
    gluQuadricNormals(elbow, GLU_SMOOTH);
    gluSphere(elbow, elbow_w, 20, 20);
    gluDeleteQuadric(elbow);
    
    // rotate forearm out and up while shrugging
    if (orient == 'l'){
        glRotatef(-shrug_height*160, 0, -1.75, 1);
    } else {
        glRotatef(shrug_height*160, 0, 1.75, 1);
    }
    // forearm
    glRotatef(elbow_angle, 1, 0, 0);
    GLUquadricObj * fore = gluNewQuadric();
    gluQuadricNormals(fore, GLU_SMOOTH);
    gluCylinder(fore, elbow_w, wrist_w + 0.2, forearm_l, 20, 20);
    gluDeleteQuadric(fore);

    
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, flesh.bytes_per_pixel,
        flesh.width, flesh.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        flesh.pixel_data);

    // wrist
    glMaterialfv(GL_FRONT, GL_DIFFUSE, skin);
    glMaterialfv(GL_FRONT, GL_AMBIENT, skin_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, skin_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, skin_shi);
    glTranslatef(0, 0, forearm_l);
    if (orient == 'l'){
        glRotatef(-shrug_height*300, -0.35, 0, 1);
    } else {
        glRotatef(shrug_height*300, -0.35, 0, 1);
    }
    glScalef(.8, 1, 1); 
    GLUquadricObj * wrist = gluNewQuadric();
    gluQuadricNormals(wrist, GLU_SMOOTH);
    gluQuadricTexture(wrist, GLU_TRUE); // Texture Coords On
    gluSphere(wrist, wrist_w, 20, 20);
    gluDeleteQuadric(wrist);
    glDeleteTextures(1, &tex);
    
    // and hand . . . 
    hand(wrist_w);
    glPopMatrix();
}
void larm(){
    glPushMatrix();
    glTranslatef(4, 0, 0);
    arm('l');
    glPopMatrix();
}
void rarm(){
    glPushMatrix();
    glTranslatef(-4, 0, 0);
    glRotatef(-180, 0, 1, 0);
    arm('r');
    glPopMatrix();
}
void top(float torso_l){
    float torso_w = 4, hip_w = 3;
    glPushMatrix();
    glTranslatef(0, 5, 0);

    glPushMatrix(); // Upper Chest
    //glRotatef(20, 1, 0, 0); 
    glScalef(1.25, 1.25, 1);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluSphere(qobj, torso_w, 20, 20);
    gluDeleteQuadric(qobj);

    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    GLUquadricObj * torso_qobj = gluNewQuadric();
    gluQuadricNormals(torso_qobj, GLU_SMOOTH);
    gluCylinder(torso_qobj, torso_w, hip_w+.25, torso_l, 20, 20);
    gluDeleteQuadric(torso_qobj);
    glPopMatrix();
    
    // hip
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p);
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, p_shi);
    glTranslatef(0, -torso_l, 0);
    
    GLUquadricObj * hip = gluNewQuadric();
    gluQuadricNormals(hip, GLU_SMOOTH);
    gluSphere(hip, hip_w, 20, 20);
    gluDeleteQuadric(hip);
    glPopMatrix();
    
    // Raise this part to shrug.
    glTranslatef(0, 1+shrug_height, 0);
    // left arm
    glMaterialfv(GL_FRONT, GL_DIFFUSE, s);
    glMaterialfv(GL_FRONT, GL_AMBIENT, s_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, s_shi);
    larm(); 
    // right arm

    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, s);
    glMaterialfv(GL_FRONT, GL_AMBIENT, s_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, s_shi);
    rarm();
    glPopMatrix();
}

void leg(){
    float hip_w = 3, knee_w = 2,  ankle_w = 1.25, toes_w = 1.2;
    float upperleg_l = 6.5, foreleg_l = 7, foot_l = 2;
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, p);
    glMaterialfv(GL_FRONT, GL_AMBIENT, p_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, p_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, p_shi);
    
    glPushMatrix();
    glTranslatef(0, 5.20, .45);
    glRotatef(95, 1, 0, 0);
    GLUquadricObj * glute_qobj = gluNewQuadric();
    gluQuadricNormals(glute_qobj, GLU_SMOOTH);
    gluCylinder(glute_qobj, knee_w, hip_w, 5, 20, 20);
    glPopMatrix();

    GLUquadricObj * hip = gluNewQuadric();
    gluQuadricNormals(hip, GLU_SMOOTH);
    gluSphere(hip, hip_w, 20, 20);
    
    glPushMatrix();
    glRotatef(90, 0, 0, 0);
    GLUquadricObj * qobj = gluNewQuadric();
    gluQuadricNormals(qobj, GLU_SMOOTH);
    gluCylinder(qobj, hip_w, knee_w, upperleg_l, 20, 20);

    glTranslatef(0, 0, upperleg_l);
    GLUquadricObj * knee_qobj = gluNewQuadric();
    gluQuadricNormals(knee_qobj, GLU_SMOOTH);
    gluSphere(knee_qobj, knee_w, 20, 20);
    
    glRotatef(20, 1, 0, 0);
    GLUquadricObj * foreleg_qobj = gluNewQuadric();
    gluQuadricNormals(foreleg_qobj, GLU_SMOOTH);
    gluCylinder(foreleg_qobj, knee_w, ankle_w + 0.25, foreleg_l, 20, 20);
   
    // ankle
    

    glMaterialfv(GL_FRONT, GL_DIFFUSE, shoe);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shoe_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shoe_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, shoe_shi);

    glTranslatef(0, 0, foreleg_l);
    glScalef(1, 1, .75);
    GLUquadricObj * ankle_qobj = gluNewQuadric();
    gluQuadricNormals(ankle_qobj, GLU_SMOOTH);
    gluSphere(ankle_qobj, ankle_w, 20, 20);
    
    //glTranslatef(0, 0, 1); 
    glScalef(.8, 1, 1); 
    glRotatef(-90, 1, 0, 0);
    GLUquadricObj * foot_qobj = gluNewQuadric();
    gluQuadricNormals(foot_qobj, GLU_SMOOTH);
    gluQuadricTexture(foot_qobj, GLU_TRUE); // Texture Coords On
    gluCylinder(foot_qobj, ankle_w, toes_w, foot_l, 20, 20);
    gluDeleteQuadric(foot_qobj);

    glTranslatef(0, 0, foot_l);
    GLUquadricObj * toes_qobj = gluNewQuadric();
    gluQuadricNormals(toes_qobj, GLU_SMOOTH);
    gluQuadricTexture(toes_qobj, GLU_TRUE); // Texture Coords On
    gluSphere(toes_qobj, toes_w, 20, 20);
    glPopMatrix();
}
void lleg(float leg_spacing, float leg_slant){
    glPushMatrix();
    glTranslatef(leg_spacing, 0, 0);
    glRotatef(leg_slant, 0, 0, 1);
    leg();
    glPopMatrix();
}
void rleg(float leg_spacing, float leg_slant){
    glPushMatrix();
    glTranslatef(-leg_spacing, 0, 0);
    glRotatef(-leg_slant, 0, 0, 1);
    leg();
    glPopMatrix();
}
void bottom(float torso_l){
    float leg_spacing = 1.5, leg_slant = 7;

    glPushMatrix();
    
    glTranslatef(0, -torso_l+1, -0.25);
    glRotatef(-10, 1, 0, 0);
    // left leg
    lleg(leg_spacing, leg_slant);
    // right leg
    rleg(leg_spacing, leg_slant);
    glPopMatrix();
}

void human(){
    float torso_l = 5;
    glMaterialfv(GL_FRONT, GL_DIFFUSE, skin);
    glMaterialfv(GL_FRONT, GL_AMBIENT, skin_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, skin_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, skin_shi);
    glPushMatrix();
    neck(); 
    glPopMatrix();
    
    glMaterialfv(GL_FRONT, GL_DIFFUSE, s);
    glMaterialfv(GL_FRONT, GL_AMBIENT, s_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, s_shi);
    top(torso_l);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, s);
    glMaterialfv(GL_FRONT, GL_AMBIENT, s_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, s_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, s_shi);
    bottom(torso_l);
}

void scene(){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    human();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
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
    float rate = 0.025;
    if (pause){
        if (shrug_up){
            pause_time +=1;
            if (pause_time > 200){
                pause = 0;
                pause_time = 0;
            }
        
        } else {
            pause_time +=1;
            if (pause_time > 80){
                pause = 0;
                pause_time = 0;
            }
        }
    } else if (shrug_up){
        if (shrug_height >= .5) {
            pause = 1;
            shrug_up = 0;
        } else
            shrug_height += rate;
    } else {
        if (shrug_height <= 0){
            pause = 1;
            shrug_up = 1;
        } else
            shrug_height -= rate;
    }

    scene();
    glFlush();
}

void keyInput(unsigned char key, int x, int y){
    switch(key){
        case KEY_ESC:   // Quit Application
            exit(0);
            break;
    }
}

void keySpecial(int key, int x, int y){
    switch(key){
        // around y axis
        case GLUT_KEY_LEFT:  // rotate left neg-x of camera
            glMatrixMode(GL_PROJECTION);
            glRotatef(speed, 0, 1, 0);
            break;
        case GLUT_KEY_RIGHT: // rotate right pos-x of camera
            glMatrixMode(GL_PROJECTION);
            glRotatef(-speed, 0, 1, 0);
            break;
        // around x axis
        case GLUT_KEY_UP: // rotate up pos-y of camera
            glMatrixMode(GL_PROJECTION);
            glRotatef(speed, 1, 0, 0);
            break;
        case GLUT_KEY_DOWN: // rotate down neg-y of camera
            glMatrixMode(GL_PROJECTION);
            glRotatef(-speed, 1, 0, 0);
            break;
        default:
            vy = 0;
            vx = 0;
            break;
    }
}

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(30, 1.0f, 50, 4000);
    glTranslatef(0, 0, -75); // back up
    glPushMatrix(); // save first view

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_amb);
    
    glLightfv(GL_LIGHT1, GL_POSITION, l1g_pos);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, l1g_diff);
    glLightfv(GL_LIGHT1, GL_SPECULAR, l1g_spec);
    
    glLightfv(GL_LIGHT2, GL_POSITION, l2_pos);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, l2_diff);
    glLightfv(GL_LIGHT2, GL_SPECULAR, l2_spec);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    glDisable(GL_CULL_FACE);

    glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("The Shrugging Human");

    initGL();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keySpecial);
    glutIdleFunc(idle);
    glutMainLoop();
}
