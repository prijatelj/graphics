/*
    oldmain.c
        Display a simple lit model of the Duquesne University Old Main
        building.
    @author: Derek S. Prijatelj
    
    TODO:
    - Light it up and set all parts to be affected by light.
    - Add windows on Main Build
    - Make Sure face of triangle tower part is red.
    - Add chapel Windows
    - Add tan line decor
    - Fix Roof of Main Build
    - Add Chapel Bell Tower
*/

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h> // using mac, therefore uses this location
#include <math.h>
#include <time.h>

#define WIN_WIDTH   500
#define WIN_HEIGHT  500

#define WINDOW_BUILD_H  4
#define WINDOW_BUILD_W  1.5

#define MAIN_HEIGHT 80
#define SPEED       5

#define KEY_ESC     27
#define KEY_W       119
#define KEY_S       115
#define KEY_A       97
#define KEY_D       100
#define KEY_SPACE   32

int win_width = WIN_WIDTH, win_height = WIN_HEIGHT;

float vx = 0, vy = 0, vz = 0;
float speed = SPEED;

//GLfloat light_pos[] = { 0.5, -1.0, -.5, 0.0 };
GLfloat light_pos[] = { 0.5, 0.5, -1.0, 0.0 };
GLfloat light_amb[] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat light_diff[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat light_spec[] = { 0.9, 0.9, 0.9, 1.0 };

GLfloat brick[] = { 0.9, 0.35, 0.3, 1.0 };
GLfloat brick_amb[] = { 0.7, 0.15, 0.1, 1.0 };
GLfloat brick_spec[] = { 0.8, 0.4, 0.3, 1.0 };
GLfloat brick_shine[] = { 50.0 };

GLfloat stone[] = { 0.7, 0.5, 0.4, 1.0 };
GLfloat stone_amb[] = { 0.7, 0.5, 0.4, 1.0 };
GLfloat stone_spec[] = { 0.7, 0.7, 0.6, 1.0 };
GLfloat stone_shine[] = { 45.0 };

GLfloat shingle[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat shingle_amb[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat shingle_spec[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat shingle_shine[] = { 55.0 };

GLfloat glass[] = { 0.3, 0.3, 0.4, 1.0 };
GLfloat glass_amb[] = { 0.3, 0.3, 0.4, 1.0 };
GLfloat glass_spec[] = { 0.4, 0.4, 0.95, 1.0 };
GLfloat glass_shine[] = { 70.0 };

GLfloat frame[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat frame_amb[] = { 0.6, 0.6, 0.6, 1.0 };
GLfloat frame_spec[] = { 0.9, 0.9, 0.9, 1.0 };
GLfloat frame_shine[] = { 50.0 };

GLfloat grass[] = { 0.1, 0.35, 0.1, 1.0 };
GLfloat grass_amb[] = { 0.1, 0.35, 0.1, 1.0 };
GLfloat grass_spec[] = { 0.3, 0.55, 0.3, 1.0 };
GLfloat grass_shine[] = { 50.0 };

// return random number between 0 and limit inclusive
int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;
    do {
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

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

/*
    rectangular prism
*/
void rect_prism(float x, float y, float z, float w, float h, float l){
    glBegin(GL_QUADS);
        glNormal3f(0,0,-1);
        // back
        glVertex3f(x-(w/2), y+(h/2), z-(l/2));
        glVertex3f(x-(w/2), y-(h/2), z-(l/2));
        glVertex3f(x+(w/2), y-(h/2), z-(l/2));
        glVertex3f(x+(w/2), y+(h/2), z-(l/2));
        // right
        glNormal3f(-1,0,0);
        glVertex3f(x+(w/2), y-(h/2), z-(l/2));
        glVertex3f(x+(w/2), y+(h/2), z-(l/2));
        glVertex3f(x+(w/2), y+(h/2), z+(l/2));
        glVertex3f(x+(w/2), y-(h/2), z+(l/2));
        // front
        glNormal3f(0,0,-1);
        glVertex3f(x+(w/2), y+(h/2), z+(l/2));
        glVertex3f(x+(w/2), y-(h/2), z+(l/2));
        glVertex3f(x-(w/2), y-(h/2), z+(l/2));
        glVertex3f(x-(w/2), y+(h/2), z+(l/2));
        // left
        glNormal3f(1,0,0);
        glVertex3f(x-(w/2), y-(h/2), z+(l/2));
        glVertex3f(x-(w/2), y+(h/2), z+(l/2));
        glVertex3f(x-(w/2), y+(h/2), z-(l/2));
        glVertex3f(x-(w/2), y-(h/2), z-(l/2));
    glEnd();
    // Top and Bottom Caps
    glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glVertex3f(x-(w/2), y+(h/2), z-(l/2));
        glVertex3f(x+(w/2), y+(h/2), z-(l/2));
        glVertex3f(x+(w/2), y+(h/2), z+(l/2));
        glVertex3f(x-(w/2), y+(h/2), z+(l/2));

        glNormal3f(0,-1,0);
        glVertex3f(x-(w/2), y-(h/2), z-(l/2));
        glVertex3f(x+(w/2), y-(h/2), z-(l/2));
        glVertex3f(x+(w/2), y-(h/2), z+(l/2));
        glVertex3f(x-(w/2), y-(h/2), z+(l/2));
    glEnd();
}

/*
    triangular prism
*/
void tri_prism(float x, float y, float z, float w, float h, float l,
    int rot){
    if (rot){
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0,-1,0); // bottom
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x-(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z-(l/2));
            
            glNormal3f(0, 1, -1); // side 1
            glVertex3f(x+(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z-(l/2));
            glVertex3f(x, y+h, z+(l/2));
            glVertex3f(x, y+h, z-(l/2));
            
            glNormal3f(0, 1, 1); // side 1
            glVertex3f(x, y+h, z-(l/2));
            glVertex3f(x, y+h, z+(l/2));
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x-(w/2), y, z+(l/2));
        glEnd();

        glColor3ub(200, 0, 0); // Red brick main box
        glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
        glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
        glBegin(GL_TRIANGLES);
            glNormal3f(0,0,-1);
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x+(w/2), y, z-(l/2));
            glVertex3f(x, y+h, z-(l/2));
            glNormal3f(0,0,-1);
            glVertex3f(x-(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z+(l/2));
            glVertex3f(x, y+h, z+(l/2));
        glEnd();
        glColor3ub(80,80,100); // shingle roof
        glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
        glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    } else {
        glBegin(GL_QUAD_STRIP);
            glNormal3f(0,-1,0); // bottom
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x+(w/2), y, z-(l/2));
            glVertex3f(x-(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z+(l/2));

            glNormal3f(1,1,0); // bottom
            glVertex3f(x-(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y, z+(l/2));
            glVertex3f(x-(w/2), y+h, z);
            glVertex3f(x+(w/2), y+h, z);
            
            glNormal3f(-1,1,0); // bottom
            glVertex3f(x-(w/2), y+h, z);
            glVertex3f(x+(w/2), y+h, z);
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x+(w/2), y, z-(l/2));
        glEnd();
        glColor3ub(200, 0, 0); // Red brick main box
        glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
        glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
        glBegin(GL_TRIANGLES);
            glNormal3f(1,0,0);
            glVertex3f(x-(w/2), y, z-(l/2));
            glVertex3f(x-(w/2), y, z+(l/2));
            glVertex3f(x-(w/2), y+h, z);
            glNormal3f(-1,0,0);
            glVertex3f(x+(w/2), y, z-(l/2));
            glVertex3f(x+(w/2), y, z+(l/2));
            glVertex3f(x+(w/2), y+h, z);
        glEnd();
        glColor3ub(80,80,100); // shingle roof
        glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
        glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
        glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
        glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    }
}

void cornerRoof(float wide, float h){
    // corner roof parts.
    glColor3ub(70,70,80); // shingle roof
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,1,0);
        glVertex3f(wide/3, MAIN_HEIGHT-5, 40.0f);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 50.0f);
        glVertex3f(wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 50.0f);
        glVertex3f(wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(1,1,1);
        glVertex3f(-wide/3, MAIN_HEIGHT-5, 40.0f);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 50.0f);
        glVertex3f(-wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 50.0f);
        glVertex3f(-wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, 30);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(1,1,-1);
        glVertex3f(-wide/3, MAIN_HEIGHT-5, -40.0f);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -50.0f);
        glVertex3f(-wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -50.0f);
        glVertex3f(-wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
        glVertex3f(-wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
        glNormal3f(-1,1,0);
        glVertex3f(wide/3, MAIN_HEIGHT-5, -40.0f);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -50.0f);
        glVertex3f(wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -50.0f);
        glVertex3f(wide/6, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
        glVertex3f(wide/2, (MAIN_HEIGHT/2)+(h*1.05/2)-1.5, -30);
    glEnd();
}

void chapel(){
    glColor3ub(200, 40, 40); // body
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(-45, MAIN_HEIGHT/2-15, -40.0f, 50, MAIN_HEIGHT-50, 30 );
    glColor3ub(180, 140, 120); // stone foundation
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, stone_spec);
    //glMaterialfv(GL_FRONT, GL_SHININESS, stone_shine);
    rect_prism(-45, 5, -40.0f, 50, 10, 30 );
    glColor3ub(100, 80, 80); // shingle
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    tri_prism(-45, MAIN_HEIGHT/2, -40.0f, 50, MAIN_HEIGHT/5, 30, 0);

    glColor3ub(180, 50, 50); // top penta box
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(-75, MAIN_HEIGHT/2-5, -40.0f, 20, MAIN_HEIGHT/6, 18 );
    glBegin(GL_QUAD_STRIP);
        glColor3ub(200, 40, 40); // top penta box
        glVertex3f(-75-10, MAIN_HEIGHT/2+2, -40-9);
        glVertex3f(-75-10, MAIN_HEIGHT/2-10, -40-9);
        glColor3ub(200, 70, 40); // top penta box
        glVertex3f(-75-15, MAIN_HEIGHT/2+2, -40-6);
        glVertex3f(-75-15, MAIN_HEIGHT/2-10, -40-6);
        glVertex3f(-75-15, MAIN_HEIGHT/2+2, -40+6);
        glVertex3f(-75-15, MAIN_HEIGHT/2-10, -40+6);
        glColor3ub(200, 40, 70); // top penta box
        glVertex3f(-75-10, MAIN_HEIGHT/2+2, -40+9);
        glVertex3f(-75-10, MAIN_HEIGHT/2-10, -40+9);
    glEnd();
    glColor3ub(80, 80, 80); // shingle
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    tri_prism(-75, MAIN_HEIGHT/2+1.75, -40.0f, 20, MAIN_HEIGHT/8, 18, 0);
    glBegin(GL_TRIANGLE_FAN);
        glColor3ub(120, 80, 80); // shingle
        glVertex3f(-75-10, MAIN_HEIGHT/2+12, -40);
        glVertex3f(-75-10, MAIN_HEIGHT/2+2, -40-9);
        glVertex3f(-75-15, MAIN_HEIGHT/2+2, -40-6);
        glColor3ub(100, 100, 80); // shingle
        glVertex3f(-75-15, MAIN_HEIGHT/2+2, -40+6);
        glVertex3f(-75-10, MAIN_HEIGHT/2+2, -40+9);
    glEnd();


    glColor3ub(190, 70, 70); // bottom penta box
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(-80, MAIN_HEIGHT/2-20, -37.5f, 22.5, MAIN_HEIGHT/6+7.5, 35 );
    glColor3ub(180, 120, 100); // bot
    rect_prism(-80, MAIN_HEIGHT/2-35, -37.5f, 22.49, MAIN_HEIGHT/8, 35);
    glBegin(GL_QUAD_STRIP);
        glColor3ub(200, 40, 40); // top penta box
        glVertex3f(-80-10, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40-11);
        glVertex3f(-80-10, MAIN_HEIGHT/2-35-MAIN_HEIGHT/6+7.5, -40-11);
        glColor3ub(200, 70, 40); // top penta box
        glVertex3f(-80-15, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40-8);
        glVertex3f(-80-15, MAIN_HEIGHT/2-35-MAIN_HEIGHT/6+7.5, -40-8);
        glVertex3f(-80-15, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40+8);
        glVertex3f(-80-15, MAIN_HEIGHT/2-35-MAIN_HEIGHT/6+7.5, -40+8);
        glColor3ub(200, 40, 70); // top penta box
        glVertex3f(-80-10, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40+11);
        glVertex3f(-80-10, MAIN_HEIGHT/2-35-MAIN_HEIGHT/6+7.5, -40+11);
    glEnd();

    glColor3ub(80, 80, 80); // shingle
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    glBegin(GL_TRIANGLE_FAN);
        glColor3ub(120, 80, 80); // shingle
        glVertex3f(-80-10, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40-11);
        glVertex3f(-80-10, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40-11);
        glVertex3f(-80-15, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40-8);
        glColor3ub(100, 100, 80); // shingle
        glVertex3f(-80-15, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40+8);
        glVertex3f(-80-10, MAIN_HEIGHT/2-30+MAIN_HEIGHT/6+7.5, -40+11);
    glEnd();
}

void window(float x, float y, float z, int plane){
    float w = WINDOW_BUILD_W, h = WINDOW_BUILD_H;// half of value.
    float f = 8/10, zfight = 0.1f;
    if (plane == 0){
        glBegin(GL_QUADS);
            // white frame
            glMaterialfv(GL_FRONT, GL_DIFFUSE, frame);
            glMaterialfv(GL_FRONT, GL_AMBIENT, frame_amb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, frame_spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, frame_shine);

            glNormal3f(0,0,-1);
            
            glVertex3f(x-w, y-h, z);
            glVertex3f(x+w, y-h, z);
            glVertex3f(x+w, y+h, z);
            glVertex3f(x-w, y+h, z);
            
            // blue glass
            
            glMaterialfv(GL_FRONT, GL_DIFFUSE, glass);
            glMaterialfv(GL_FRONT, GL_AMBIENT, glass_amb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, glass_spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, glass_shine);

            glVertex3f(x-w*.8, y - h*.1, z + zfight);
            glVertex3f(x+w*.8, y - h*.1, z + zfight);
            glVertex3f(x+w*.8, y - h*.9, z + zfight);
            glVertex3f(x-w*.8, y - h*.9, z + zfight);
            glVertex3f(x-w*.8, y + h*.1, z + zfight);
            glVertex3f(x+w*.8, y + h*.1, z + zfight);
            glVertex3f(x+w*.8, y + h*.9, z + zfight);
            glVertex3f(x-w*.8, y + h*.9, z + zfight);

        glEnd();
    } else if (plane == 1){
        glBegin(GL_QUADS);
            // white frame
            glMaterialfv(GL_FRONT, GL_DIFFUSE, frame);
            glMaterialfv(GL_FRONT, GL_AMBIENT, frame_amb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, frame_spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, frame_shine);

            glNormal3f(0,0,-1);
            
            glVertex3f(x, y-h, z-w);
            glVertex3f(x, y-h, z+w);
            glVertex3f(x, y+h, z+w);
            glVertex3f(x, y+h, z-w);
            
            // blue glass
            glMaterialfv(GL_FRONT, GL_DIFFUSE, glass);
            glMaterialfv(GL_FRONT, GL_AMBIENT, glass_amb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, glass_spec);
            glMaterialfv(GL_FRONT, GL_SHININESS, glass_shine);
            
            glVertex3f(x + zfight, y - h*.1, z-w*.8);
            glVertex3f(x + zfight, y - h*.1, z+w*.8);
            glVertex3f(x + zfight, y - h*.9, z+w*.8);
            glVertex3f(x + zfight, y - h*.9, z-w*.8);
            glVertex3f(x + zfight, y + h*.1, z-w*.8);
            glVertex3f(x + zfight, y + h*.1, z+w*.8);
            glVertex3f(x + zfight, y + h*.9, z+w*.8);
            glVertex3f(x + zfight, y + h*.9, z-w*.8);
        glEnd();
    }
}

/*
    Windows
*/
void windows(){
    // Main build back
    // mid
    window(0.0, 16.0, 55.1, 0);
    window(5.0, 16.0, 55.1, 0);
    window(-5.0, 16.0, 55.1, 0);

    window(0.0, 28.0, 55.1, 0);
    window(5.0, 28.0, 55.1, 0);
    window(-5.0, 28.0, 55.1, 0);

    window(0.0, 40.0, 55.1, 0);
    window(5.0, 40.0, 55.1, 0);
    window(-5.0, 40.0, 55.1, 0);

    window(0.0, 52.0, 55.1, 0);
    window(5.0, 52.0, 55.1, 0);
    window(-5.0, 52.0, 55.1, 0);

    window(0.0, 64.0, 55.1, 0);
    window(5.0, 64.0, 55.1, 0);
    window(-5.0, 64.0, 55.1, 0);

    window(2.5, 76.0, 55.1, 0);
    window(-2.5, 76.0, 55.1, 0);

    // right
    window(13.0, 16.0, 50.1, 0);
    window(18.0, 16.0, 50.1, 0);

    window(13.0, 28.0, 50.1, 0);
    window(18.0, 28.0, 50.1, 0);

    window(13.0, 40.0, 50.1, 0);
    window(18.0, 40.0, 50.1, 0);

    window(13.0, 52.0, 50.1, 0);
    window(18.0, 52.0, 50.1, 0);

    window(13.0, 64.0, 50.1, 0);
    window(18.0, 64.0, 50.1, 0);

    // left
    window(-13.0, 16.0, 50.1, 0);
    window(-18.0, 16.0, 50.1, 0);

    window(-13.0, 28.0, 50.1, 0);
    window(-18.0, 28.0, 50.1, 0);

    window(-13.0, 40.0, 50.1, 0);
    window(-18.0, 40.0, 50.1, 0);

    window(-13.0, 52.0, 50.1, 0);
    window(-18.0, 52.0, 50.1, 0);

    window(-13.0, 64.0, 50.1, 0);
    window(-18.0, 64.0, 50.1, 0);

    // Side of building
    // left
    window(22.6, 16.0, 46, 1);
    window(22.6, 16.0, 40, 1);
    window(22.6, 16.0, 34, 1);

    window(22.6, 28.0, 46,  1);
    window(22.6, 28.0, 40, 1);
    window(22.6, 28.0, 34, 1);

    window(22.6, 40.0, 46, 1);
    window(22.6, 40.0, 40, 1);
    window(22.6, 40.0, 34, 1);

    window(22.6, 52.0, 46, 1);
    window(22.6, 52.0, 40, 1);
    window(22.6, 52.0, 34, 1);

    window(22.6, 64.0, 46, 1);
    window(22.6, 64.0, 40, 1);
    window(22.6, 64.0, 34, 1);
    
    // mid
    window(25.1, 16.0, 6, 1);
    window(25.1, 16.0, 0, 1);
    window(25.1, 16.0, -6, 1);

    window(25.1, 28.0, 6,  1);
    window(25.1, 28.0, 0, 1);
    window(25.1, 28.0, -6, 1);

    window(25.1, 40.0, 6, 1);
    window(25.1, 40.0, 0, 1);
    window(25.1, 40.0, -6, 1);

    window(25.1, 52.0, 6, 1);
    window(25.1, 52.0, 0, 1);
    window(25.1, 52.0, -6, 1);

    window(25.1, 64.0, 6, 1);
    window(25.1, 64.0, 0, 1);
    window(25.1, 64.0, -6, 1);
    
    window(25.1, 76.0, 2, 1);
    window(25.1, 76.0, -2, 1);

    // right
    window(22.6, 16.0, -34, 1);
    window(22.6, 16.0, -40, 1);
    window(22.6, 16.0, -46, 1);

    window(22.6, 28.0, -34,  1);
    window(22.6, 28.0, -40, 1);
    window(22.6, 28.0, -46, 1);

    window(22.6, 40.0, -34, 1);
    window(22.6, 40.0, -40, 1);
    window(22.6, 40.0, -46, 1);

    window(22.6, 52.0, -34, 1);
    window(22.6, 52.0, -40, 1);
    window(22.6, 52.0, -46, 1);

    window(22.6, 64.0, -34, 1);
    window(22.6, 64.0, -40, 1);
    window(22.6, 64.0, -46, 1);

    // mid right
    window(20.1, 16.0, -14, 1);
    window(20.1, 16.0, -20, 1);
    window(20.1, 16.0, -26, 1);

    window(20.1, 28.0, -14,  1);
    window(20.1, 28.0, -20, 1);
    window(20.1, 28.0, -26, 1);

    window(20.1, 40.0, -14, 1);
    window(20.1, 40.0, -20, 1);
    window(20.1, 40.0, -26, 1);

    window(20.1, 52.0, -14, 1);
    window(20.1, 52.0, -20, 1);
    window(20.1, 52.0, -26, 1);

    window(20.1, 64.0, -14, 1);
    window(20.1, 64.0, -20, 1);
    window(20.1, 64.0, -26, 1);
    
    // mid left
    window(20.1, 16.0, 26, 1);
    window(20.1, 16.0, 20, 1);
    window(20.1, 16.0, 14, 1);

    window(20.1, 28.0, 26,  1);
    window(20.1, 28.0, 20, 1);
    window(20.1, 28.0, 14, 1);

    window(20.1, 40.0, 26, 1);
    window(20.1, 40.0, 20, 1);
    window(20.1, 40.0, 14, 1);

    window(20.1, 52.0, 26, 1);
    window(20.1, 52.0, 20, 1);
    window(20.1, 52.0, 14, 1);

    window(20.1, 64.0, 26, 1);
    window(20.1, 64.0, 20, 1);
    window(20.1, 64.0, 14, 1);
    
    // chapel thing...
    window(-85.1, 16.0, -19.9, 0);
    window(-80.1, 16.0, -19.9, 0);
    window(-75.1, 16.0, -19.9, 0);

    window(-80.1, 35.0, -30.9, 0);
    window(-75.1, 35.0, -30.9, 0);
}


/*
    environment() : Draws the surface the die will roll on.
*/
void environment(){
    // draw flat surace.
    glColor3ub(128, 128, 128);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
    glMaterialfv(GL_FRONT, GL_AMBIENT, grass_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, grass_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, grass_shine);
    glBegin(GL_QUADS);
        glNormal3f(0,-1,0);
        glVertex3f(1000, 0, 1000);
        glVertex3f(-1000, -10, 1000);
        glVertex3f(-1000, -10, -1000);
        glVertex3f(1000, 20, -1000);
    glEnd();
}

/*
    building:   creates the old main building centered at origin.
*/
void building(){
    // main Body:
    float h = MAIN_HEIGHT-20;
    float wide = 45.0f;

    glColor3ub(200, 0, 0); // Red brick main box
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(0.0f, MAIN_HEIGHT/2, 0.0f, 40.0f, h, 80.0f);
    glColor3ub(180, 150, 100); // stone base
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
    glMaterialfv(GL_FRONT, GL_AMBIENT, stone_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, stone_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, stone_shine);
    rect_prism(0.0f, 5, 0.0f, 40.0f, 10, 80.0f);
    glColor3ub(80,80,100); // shingle roof
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    tri_prism(0.0f, h+10, 0.0f, 40.0f, h/3, 80.0f, 1);
    
    glColor3ub(180, 0, 0); // Red END boxes
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(0.0f, MAIN_HEIGHT/2, 40.0f, wide, h +.01, 20.0f);
    rect_prism(0.0f, MAIN_HEIGHT/2, -40.0f, wide, h +.01, 20.0f);
    glColor3ub(180, 150, 100);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
    glMaterialfv(GL_FRONT, GL_AMBIENT, stone_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, stone_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, stone_shine);
    rect_prism(0.0f, 5, 40.0f, wide, 10, 20.0f);
    rect_prism(0.0f, 5, -40.0f, wide, 10, 20.0f);
    
    glColor3ub(220, 0, 0); // Red
    glMaterialfv(GL_FRONT, GL_DIFFUSE, brick);
    glMaterialfv(GL_FRONT, GL_AMBIENT, brick_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, brick_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, brick_shine);
    rect_prism(0.0f, (MAIN_HEIGHT/2) +h*.025, 0.0f, 50.0f, h*1.05, 20.0f);
    rect_prism(0.0f, (MAIN_HEIGHT/2) +h*.025, 0.0f, 20.0f, h*1.05, 110.0f);
    glColor3ub(180, 150, 100);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, stone);
    glMaterialfv(GL_FRONT, GL_AMBIENT, stone_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, stone_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, stone_shine);
    rect_prism(0.0f, 5, 0.0f, 50.0f, 10, 20.0f);
    rect_prism(0.0f, 5, 0.0f, 20.0f, 10, 110.0f);
    glColor3ub(90,90,110); // shingle roof
    glMaterialfv(GL_FRONT, GL_DIFFUSE, shingle);
    glMaterialfv(GL_FRONT, GL_AMBIENT, shingle_amb);
    glMaterialfv(GL_FRONT, GL_SPECULAR, shingle_spec);
    glMaterialfv(GL_FRONT, GL_SHININESS, shingle_shine);
    tri_prism(0.0f, h+13, 44.99f, 20.0f, h/5, 20.0f, 1);
    tri_prism(0.0f, h+13, -44.99f, 20.0f, h/5, 20.0f, 1);
    tri_prism(0.0f, h+13, 0.0f, 50, h/5, 20.0f, 0);
    
    cornerRoof(wide, h);

    // chapel:
    chapel();

    // decor:
    windows();
}


void keyInput(unsigned char key, int x, int y){
    switch(key){
        case KEY_ESC:   // Quit Application
            exit(0);
            break;
        case KEY_W: // Zoom In
            vz = speed;
            glTranslatef(0, 0, vz);
            break;
        case KEY_S: // Zoom Out
            vz = -speed;
            glTranslatef(0, 0, vz);
            break;
        case KEY_A: // Z rotate CW
            vz = -speed;
            glTranslatef(vz, 0, 0);
            break;
        case KEY_D: // Z rotate CCW
            vz = -speed;
            glTranslatef(-vz, 0, 0);
            break;
        case KEY_SPACE:
            glPopMatrix();
            glPushMatrix();
        default:
            vz = 0;
            break;
    }
}

void keySpecial(int key, int x, int y){
    switch(key){
        // around y axis
        case GLUT_KEY_LEFT:  // rotate left neg-x of camera
            glRotatef(speed, 0, 1, 0);
            break;
        case GLUT_KEY_RIGHT: // rotate right pos-x of camera
            glRotatef(-speed, 0, 1, 0);
            break;
        // around x axis
        case GLUT_KEY_UP: // rotate up pos-y of camera
            glRotatef(speed, 1, 0, 0);
            break;
        case GLUT_KEY_DOWN: // rotate down neg-y of camera
            glRotatef(-speed, 1, 0, 0);
            break;
        default:
            vy = 0;
            vx = 0;
            break;
    }
}

/*
    TODO: Enable standard mouse controls for rotating and zooming view
*/
void mouse(){
    
}

void scene(){
    environment();
    building();
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
    glFlush();
}

void idle(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene();
    glFlush();
}
void reshape(GLsizei w, GLsizei h){
    glutReshapeWindow(win_width,win_height);
}

void initGL(){
    glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    gluPerspective(30, 1.0f, 50, 4000);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0, -MAIN_HEIGHT, -MAIN_HEIGHT*4);
    glPushMatrix(); // save first view

    //*
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    //glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_amb);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //*/
    glShadeModel(GL_SMOOTH);

    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    srand(time(NULL));
}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Duquesne University Old Main");

    initGL();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keySpecial);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
}
