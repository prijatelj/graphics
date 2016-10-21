/*
    dice.c
        Display a 3d die.
    @author: Derek S. Prijatelj
    
    TODO:
    -   Rotate via keys or mouse input.
    -   Implement ability to choose which i# sided die is displayed
    -   proper texturing of all # sided dice
    -   include dice roll animation
*/

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h> // using mac, therefore uses this location
#include <math.h>
#include <time.h>

#define WIN_WIDTH   500
#define WIN_HEIGHT  500

#define CUBE_SIZE   50
#define SPEED       10

#define KEY_ESC     27
#define KEY_W       119
#define KEY_S       115
#define KEY_A       97
#define KEY_D       100
#define KEY_SPACE   32

int win_width = WIN_WIDTH, win_height = WIN_HEIGHT;

float vx = 0, vy = 0, vz = 0;
float speed = SPEED;

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
    environment() : Draws the surface the die will roll on.
*/
void environment(){
    // draw flat surace.
    glColor3ub(128, 128, 128);
    glBegin(GL_QUADS);
        glVertex3f(1000, CUBE_SIZE/-2, 1000);
        glVertex3f(-1000, CUBE_SIZE/-2, 1000);
        glVertex3f(-1000, CUBE_SIZE/-2, -1000);
        glVertex3f(1000, CUBE_SIZE/-2, -1000);
    glEnd();
}

/*
    dice() : Draws the current die or dice.
    TODO : be versatile, not a static 6 sided die w/ 1 on top only.
*/
void dice(){
    glColor3ub(255, 0, 0); // Red die
    glutSolidCube(CUBE_SIZE-0.1f); // -.1 to show circles.
    
    float radius = CUBE_SIZE/9;

    // draw circles on respective sides.
    // front face is 2
    glColor3ub(255, 255, 255); // Dark Red w/ blue tint circle.
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        0, CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        0, CUBE_SIZE/2);

    // Back Face is 5
    equilateralPolygon(radius, 75, 0,
                        0, 0, 1, 1,
                        0, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        0, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        0, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        0, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        0, -CUBE_SIZE/2);
    
    // Top Face is 1
    equilateralPolygon(radius, 75, 0,
                        0, 0, 1, 1,
                        1, CUBE_SIZE/2);

    // Right Face is 3
    equilateralPolygon(radius, 75, 0,
                        0, 0, 1, 1,
                        2, CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        2, CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        2, CUBE_SIZE/2);
    
    // Left Face is 4
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        2, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, CUBE_SIZE/4, 1, 1,
                        2, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        -CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        2, -CUBE_SIZE/2);
    equilateralPolygon(radius, 75, 0,
                        CUBE_SIZE/4, -CUBE_SIZE/4, 1, 1,
                        2, -CUBE_SIZE/2);
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


void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    environment();
    dice();
    glFlush();
}

void idle(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    environment();
    dice();
    

    glFlush();
}
void reshape(GLsizei w, GLsizei h){
    glutReshapeWindow(win_width,win_height);
}

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    gluPerspective(40, 1.0f, 25, 8000);
    glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    glTranslatef(0, 0, -CUBE_SIZE*3);
    glPushMatrix(); // save first view

    glClear(GL_COLOR_BUFFER_BIT);
    srand(time(NULL));
}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    //glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA );
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("3d Dice Roller");

    initGL();
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(keySpecial);
    glutMouseFunc(mouse);
    glutIdleFunc(idle);
    glutMainLoop();
}
