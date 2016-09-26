/*
    triach.c
        A simple 2d game. You play as the archer Triarch as he flees from 
        the royal army. This is a rudimentary version and is incomplete.
    @author: Derek S. Prijatelj
*/

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h> // using mac, therefore uses this location
#include <math.h>
#include <time.h>
#include "triarch.h"

/*
    triangle sprite to represent Triarch
    rectangles for quivers of arrows that his companions have placed ahead
        of time for his escape.
    
    TODO:
    -   implement a shooting mechanic, ammunition of arrows
    -   implement enemy sprites coming down from above
    -   obstacles, such as trees and rocks
    -   enemy horder below, pursuing triarch (Game over if fall below them)
    -   enemy archers
    -   health potions along with Hit points
    -   Main Menu, High Scores, save states.
*/

#define WIN_W       500.0
#define WIN_H       500.0

//directional keys
#define KEY_SPACE   0x20
#define KEY_ESC     0x1B

#define TRI_SIZE    60.0
#define TRI_SPEED   45.0
#define TRI_HP      3

#define Q_WIDTH     60.0
#define Q_HEIGHT    20.0
#define Q_QUANTITY  600
#define Q_FALL_RATE 7
#define Q_SPAWN_T   9000

time_t start = 0;
int rand_lim(int);

int arrows = 0;
int max_arrows = 0;
int max_plus = 0;
int plus = 0;

float win_width = WIN_W;
float win_height = WIN_H;

void end_game(int dead){
    puts("\nGame Results:");
    if (dead)
        puts("You Died.");
    else
        puts("You Quit.");
    printf("You survived for %.0f seconds\n\n", difftime(time(0), start));
    printf("Total Walls Dodged: %d\n", max_arrows - arrows);
    printf("Total Wall Collisions: %d\n", arrows);
    printf("Maximum Walls: %d\n\n", max_arrows);
    printf("Blue Blocks Collected: %d\n", plus);
    printf("Maximum Blue Blocks: %d\n\n", max_plus);
    exit(0);
}

typedef struct {
    int hp;
    float x;        // center position on horizontal axis
    float size;     // length of one of triarch's sides
    float height;
} player;
player triarch = {TRI_HP, WIN_W/2, TRI_SIZE,
    sqrt(pow(TRI_SIZE,2)-pow(TRI_SIZE/2,2))};

void triarchDisplay(){
    glColor3ub(255, 50, 50); // Red Triangle.
    glBegin(GL_TRIANGLES);
        glVertex2f(triarch.x - triarch.size/2, 0.0f);
        glVertex2f(triarch.x + triarch.size/2, 0.0f);
        glVertex2f(triarch.x, triarch.height);
    glEnd();
}

typedef struct {
    float w, h, x, y;
    int state;      // 0: DNE, 1: Normal, 2: Just Picked Up
} quiver;
quiver quivers[Q_QUANTITY] = { [0 ... Q_QUANTITY-1] =
    {Q_WIDTH, Q_HEIGHT, WIN_W/4, WIN_H/4, 0} };

float sign(float p1x, float p1y,
            float p2x, float p2y,
            float p3x, float p3y ){
    return (p1x - p3x) * (p2y - p3y) - (p2x - p3x) * (p1y - p3y);
}

int point_in_tri( float p1x, float p1y,
            float v1x, float v1y,
            float v2x, float v2y,
            float v3x, float v3y ){
   int b1 = sign(p1x, p1y, v1x, v1y, v2x, v2y) <= 0.0f; 
   int b2 = sign(p1x, p1y, v2x, v2y, v3x, v3y) <= 0.0f; 
   int b3 = sign(p1x, p1y, v3x, v3y, v1x, v1y) <= 0.0f; 
   return ((b1 == b2) && (b2 == b3));
}

int q_close2_tri(quiver q){
    return 1;
    /*
    return q.y <= triarch.height && ( (q.x <= triarch.x + (triarch.size/2)
        && q.x >= triarch.x - (triarch.size/2)) || (q.x+q.w <= triarch.x +
        (triarch.size/2) && q.x+q.w >= triarch.x - (triarch.size/2)) );
    */
}

int point_in_rect(int px, int py, int rx, int ry, int rw, int rh){
    return (px >= rx && px <= rx+rw ) && (py >= ry && py <= ry+rh);
}

int q_intersect_tri(quiver q){
    float tri_left = triarch.x - (triarch.size/2);
    float tri_right = triarch.x + (triarch.size/2);
    /* a triangle point is not inside the rectangle. Check if rectangle
        point inside triangle.*/
    int xy = point_in_tri(q.x, q.y, tri_left, 0, tri_right, 0,
        triarch.x, triarch.height);
    int xwy = point_in_tri(q.x + q.w, q.y, tri_left, 0, tri_right, 0,
        triarch.x, triarch.height);
    int xyh = point_in_tri(q.x, q.y + q.h, tri_left, 0, tri_right, 0,
        triarch.x, triarch.height);
    int xwyh = point_in_tri(q.x + q.w, q.y + q.h, tri_left, 0,
        tri_right, 0, triarch.x, triarch.height);
    if ( !(xy || xwy || xyh || xwyh) ){
        int tri_left_in = point_in_rect(tri_left, 0, q.x, q.y, q.w, q.h);
        int tri_right_in = point_in_rect(tri_right, 0, q.x, q.y, q.w, q.h);
        int tri_mid_in = point_in_rect(triarch.x, triarch.height,
            q.x, q.y, q.w, q.h);
        int tri_center_in = point_in_rect(triarch.x, triarch.height/2,
            q.x, q.y, q.w, q.h);
        return tri_left_in || tri_right_in || tri_mid_in || tri_center_in;
    }
    else
        return 1;
}
void quiverDisplay(){
    for (int i = 0; i < Q_QUANTITY; i++){
        switch(quivers[i].state){
            case 3:
            case 0: // randomly spawn
                if (rand_lim(Q_SPAWN_T) == 0 ){
                    // generate random horizontal position.
                    quivers[i].x = rand_lim(win_width - Q_WIDTH);
                    quivers[i].y = win_height;
                    if (quivers[i].state < 3){
                        glColor4ub(200, 100, 0, 255);
                        quivers[i].state = 1;
                        max_arrows++;
                    }
                    else {
                        glColor4ub(100, 150, 255, 255);
                        quivers[i].state = 4;
                        max_plus++;
                    }
                    glRectf(quivers[i].x, quivers[i].y,
                        quivers[i].x + quivers[i].w,
                        quivers[i].y + quivers[i].h );
                }
                break;
            case 4:
            case 1: // check if intersects player sprite
                // check if within distance first
                if (q_close2_tri(quivers[i])){
                    // if intersect change color. state = 2;
                    if (q_intersect_tri(quivers[i])){
                        if (quivers[i].state < 3){
                            quivers[i].state = 2;
                            glColor4ub(255, 255, 50, 255);
                            glRectf(quivers[i].x, quivers[i].y,
                                quivers[i].x + quivers[i].w,
                                quivers[i].y + quivers[i].h );
                            arrows++;
                            triarch.hp--;
                        }
                        else{
                            quivers[i].state = 5;
                            glColor4ub(150, 255, 255, 255);
                            glRectf(quivers[i].x, quivers[i].y,
                                quivers[i].x + quivers[i].w,
                                quivers[i].y + quivers[i].h );
                            triarch.hp++;
                            plus++;
                        }
                            
                        if (triarch.hp <= 0)
                            end_game(1);
                        else if (triarch.hp > TRI_HP)
                            triarch.hp = TRI_HP;
                    }
                    else {
                        quivers[i].y -= Q_FALL_RATE;
                        if (quivers[i].state < 3)
                            glColor4ub(200, 100, 0, 255);
                        else
                            glColor4ub(100, 150, 255, 255);
                        glRectf(quivers[i].x, quivers[i].y,
                            quivers[i].x + quivers[i].w,
                            quivers[i].y + quivers[i].h );
                    }
                }
                else if (quivers[i].y + quivers[i].h < 0){ // Missed
                    if (quivers[i].state < 3){
                        quivers[i].state = 0;
                    }
                    else{
                        quivers[i].state = 3;
                    }
                }
                else{ // if not intersect then free fall.
                    quivers[i].y -= Q_FALL_RATE;
                    if (quivers[i].state < 3)
                        glColor4ub(200, 100, 0, 255);
                    else
                        glColor4ub(100, 150, 255, 255);
                    glRectf(quivers[i].x, quivers[i].y,
                        quivers[i].x + quivers[i].w,
                        quivers[i].y + quivers[i].h );
                }
                break;
            case 2:
                // clear from board. state = 0;
                quivers[i].state = 0;
                break;
            case 5:
                // clear from board. state = 3;
                quivers[i].state = 3;
                break;
        }
    }
}

// return random number between 0 and limit inclusive
int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;
    do { 
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

void initGL(){
    glClearColor(0.0f, 0.25f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, win_width, 0.0f, win_height);

    srand(time(NULL));
}

void reshape(GLsizei w, GLsizei h){
    glutReshapeWindow(win_width,win_height);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    // display sprites in proper location
    triarchDisplay();
    quiverDisplay();
    glFlush();
}

void idle(){
    glClear(GL_COLOR_BUFFER_BIT);
    // redraw sprites.
    triarchDisplay();
    quiverDisplay();
    glFlush();
}


void keyInput(unsigned char key, int x, int y){
    switch(key){
        case KEY_ESC:   // Quit Game.
            end_game(0);
            exit(0);
            break;
        case KEY_SPACE: // Triarch Shoot Arrow
            break;
    }
}

void keySpecial(int key, int x, int y){
    switch(key){
        case GLUT_KEY_LEFT:  // Move Triarch Left
            if (triarch.x - (triarch.size/2) > 0){
                if ((triarch.x - (triarch.size/2)) - TRI_SPEED > 0){
                    triarch.x -= TRI_SPEED;
                }
                else{
                    triarch.x = triarch.size/2;
                }
            }
            break;
        case GLUT_KEY_RIGHT: // Move Triarch Right
            if (triarch.x + (triarch.size/2) < win_width){
                if (triarch.x + (triarch.size/2) + TRI_SPEED < win_width){
                    triarch.x += TRI_SPEED;
                }
                else{
                    triarch.x = win_width - triarch.size/2;
                }
            }
            break;
    }
}

void main(int argc, char **argv){
    /*
    Include game rolls prompt in terminal, then user inserts 'start' to play
    the game. After a round is over, display close and display results in 
    terminal.
    */
    start = time(0);
    int good = floor(Q_QUANTITY/ 10);
    for (int i = 0; i < good; i++){
        quivers[i].state = 3;
    }
    //if(){ 
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
        glutInitWindowSize(win_width, win_height);
        glutCreateWindow("Triarch");
        initGL();
        glutReshapeFunc(reshape);
        glutDisplayFunc(display);
        glutKeyboardFunc(keyInput);
        glutSpecialFunc(keySpecial);
        glutIdleFunc(idle);
        glutMainLoop();
    //}
}
