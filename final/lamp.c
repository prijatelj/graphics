/*
 * lamp.c : street lamp model to be used in street_spirit
 */

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h> // using mac, therefore uses this location
//#include <GL/glut.h> // Linux
// -lglut -lGL -lGLU -lm // Linux gcc flags

#include "img/metallic.c"
#include "img/cage.c"

#define LAMP_RADIUS  2

// Light source from lamp
GLfloat lamp_light_pos[] = { -100, 100.0, 100.0, 1.0 }; // x, y, z, w~
GLfloat lamp_light_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat lamp_light_dif[] = { 0.95, 0.95, 0.95, 1.0 };
GLfloat lamp_light_spc[] = { 1.0, 1.0, 1.0, 1.0 };

// Materials
// metal
GLfloat lamp_dif[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat lamp_spc[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat lamp_shi[] = { 75.0 };
GLfloat lamp_emi[] = { 0.0, 0.0, 0.0, 1.0 };
//GLfloat lamp_emi[] = { 1.0, 1.0, 1.0, 1.0 };
// light bulb
GLfloat lamp_bulb_dif[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat lamp_bulb_spc[] = { 0.45, 0.35, 0.35, 1.0 };
GLfloat lamp_bulb_shi[] = { 75.0 };
GLfloat lamp_bulb_emi[] = { 0.99, 0.7, 0.7, 1.0 };

void spotlight(int id_L){
    switch(id_L){
        case 1:
            glLightfv(GL_LIGHT1, GL_POSITION, lamp_light_pos);
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp_light_dif);
            glLightfv(GL_LIGHT1, GL_AMBIENT, lamp_light_amb);
            glLightfv(GL_LIGHT1, GL_SPECULAR, lamp_light_spc);
            break;
        default:
            break;
    }
}
void lantern(float width, int id_L){
    // bottom of lantern
    GLUquadricObj * bot_face_qobj = gluNewQuadric();
    gluQuadricNormals(bot_face_qobj, GLU_SMOOTH);
    gluQuadricTexture(bot_face_qobj, GLU_TRUE);
    gluDisk(bot_face_qobj, 0, width/1.5, 60, 60);
    gluDeleteQuadric(bot_face_qobj);
    
    GLUquadricObj * bot_qobj = gluNewQuadric();
    gluQuadricNormals(bot_qobj, GLU_SMOOTH);
    gluQuadricTexture(bot_qobj, GLU_TRUE);
    gluCylinder(bot_qobj, width/1.5, width, 1, 60, 60);
    gluDeleteQuadric(bot_qobj);
   
    glTranslatef(0, 0, 0); 
    
    // Lantern Cage
    float case_radius = 2;
    
    //*
    GLuint bulb_tex;
    glGenTextures(1, &bulb_tex);
    glBindTexture(GL_TEXTURE_2D, bulb_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, cage.bytes_per_pixel,
        cage.width, cage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        cage.pixel_data);
   
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lamp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, lamp_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, lamp_emi);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.2);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    //glScalef(1, 1, 0.75);
    glTranslatef(0, 0, 1);
    GLUquadricObj * case_qobj = gluNewQuadric();
    gluQuadricNormals(case_qobj, GLU_SMOOTH);
    gluQuadricTexture(case_qobj, GLU_TRUE);
    gluCylinder(case_qobj, width, width, case_radius, 60, 60);
    //gluSphere(case_qobj, case_radius, 60, 60);
    gluDeleteQuadric(case_qobj);
    glPopMatrix();
    glDeleteTextures(1, &bulb_tex);
    //*/

    // needs to translate the smashed diameter
    //glTranslatef(0, 0, case_radius*.75);
    glTranslatef(0, 0, case_radius);
    
    // light bulb

    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lamp_bulb_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_bulb_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, lamp_bulb_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, lamp_bulb_emi);
    
    GLUquadricObj * light_bulb_qobj = gluNewQuadric();
    gluQuadricNormals(light_bulb_qobj, GLU_SMOOTH);
    gluQuadricTexture(light_bulb_qobj, GLU_FALSE);
    gluSphere(light_bulb_qobj, width/3, 20, 20);
    gluDeleteQuadric(light_bulb_qobj);
    
    //spotlight(id_L);// create actual spot light object: Makes game look cool.

    glTranslatef(0, 0, case_radius*.5);
    
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   
    // top of lantern
    //*
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);
    glBindTexture(GL_TEXTURE_2D, norm_tex);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D, 0, metallic.bytes_per_pixel,
        metallic.width, metallic.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        metallic.pixel_data);
   
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lamp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, lamp_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, lamp_emi);
    //*/    

    GLUquadricObj * top1_qobj = gluNewQuadric();
    gluQuadricNormals(top1_qobj, GLU_SMOOTH);
    gluQuadricTexture(top1_qobj, GLU_TRUE);
    gluCylinder(top1_qobj, width, width/1.5, 0.75, 60, 60);
    gluDeleteQuadric(top1_qobj);

    glTranslatef(0, 0, 0.75);
    GLUquadricObj * top2_qobj = gluNewQuadric();
    gluQuadricNormals(top2_qobj, GLU_SMOOTH);
    gluQuadricTexture(top2_qobj, GLU_TRUE);
    gluDisk(top2_qobj, 0, width/1.5, 60, 60);
    gluDeleteQuadric(top2_qobj);

    GLUquadricObj * cone_qobj = gluNewQuadric();
    gluQuadricNormals(cone_qobj, GLU_SMOOTH);
    gluQuadricTexture(cone_qobj, GLU_TRUE);
    gluCylinder(cone_qobj, width/7, 0, 0.5, 60, 60); // cone decoration
    gluDeleteQuadric(cone_qobj);
    
    glTranslatef(0, 0, 0.5);
    GLUquadricObj * topper_qobj = gluNewQuadric();
    gluQuadricNormals(topper_qobj, GLU_SMOOTH);
    gluQuadricTexture(topper_qobj, GLU_TRUE);
    gluSphere(topper_qobj, width/7, 60, 60);
    gluDeleteQuadric(topper_qobj);
    glDeleteTextures(1, &norm_tex);    
}
void lamp(int id_L){
    float width = LAMP_RADIUS, stem_l = 5, base1_l = 1, base2_l = 0.5;
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    // base
    // flat cyl
   
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
    glTexImage2D(GL_TEXTURE_2D, 0, metallic.bytes_per_pixel,
        metallic.width, metallic.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        metallic.pixel_data);
   
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lamp_dif);
    glMaterialfv(GL_FRONT, GL_SPECULAR, lamp_spc);
    glMaterialfv(GL_FRONT, GL_SHININESS, lamp_shi);
    glMaterialfv(GL_FRONT, GL_EMISSION, lamp_emi);

    GLUquadricObj * base1_qobj = gluNewQuadric();
    gluQuadricNormals(base1_qobj, GLU_SMOOTH);
    gluQuadricTexture(base1_qobj, GLU_TRUE);
    gluCylinder(base1_qobj, width, width, base1_l, 60, 60);
    gluDeleteQuadric(base1_qobj);

    glTranslatef(0, 0, base1_l); 
    GLUquadricObj * base2_qobj = gluNewQuadric();
    gluQuadricNormals(base2_qobj, GLU_SMOOTH);
    gluQuadricTexture(base2_qobj, GLU_TRUE);
    gluCylinder(base2_qobj, width, width/4, base2_l, 60, 60);
    gluDeleteQuadric(base2_qobj);
    
    glTranslatef(0, 0, base2_l); 
    GLUquadricObj * stem_qobj = gluNewQuadric();
    gluQuadricNormals(stem_qobj, GLU_SMOOTH);
    gluQuadricTexture(stem_qobj, GLU_TRUE);
    gluCylinder(stem_qobj, width/4, width/4, stem_l, 60, 60);
    gluDeleteQuadric(stem_qobj);

    glTranslatef(0, 0, stem_l); 
    lantern(width, id_L);

    glDeleteTextures(1, &tex);
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
