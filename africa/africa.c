/*
    africa.c
        Utilizes OpenGL to display a political map of Africa. When the user
        clicks on a region of one of the countries of Africa, the title bar
        will change to the name of the country selected.
    author: Derek S. Prijatelj
*/

#include <stdio.h>
#include <stdlib.h>
#include <glut/glut.h>
#include <time.h>
#include "map.c"

int width = 512;
int height = 512;

void initGL(){
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 
    srand(time(NULL));
}

void reshape(GLsizei w, GLsizei h){
    glutReshapeWindow(width,height);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    GLuint image;
    
    /* assign it a reference. You can use any number */
    glGenTextures(1, &image);

    /* load the image into memory. Replace gimp_image with whatever the 
        array is called in the .c file */
    gluBuild2DMipmaps(GL_TEXTURE_2D, gimp_image.bytes_per_pixel,
        gimp_image.width, gimp_image.height, GL_RGBA, GL_UNSIGNED_BYTE,
        gimp_image.pixel_data);

    /* enable texturing. If you don't do this, you won't get any image
         displayed */
    glEnable(GL_TEXTURE_2D);

    /* draw the texture to the screen, on a white box from (0,0) to (1, 1).
        Other shapes may be used. */
    glColor3f(1.0, 1.0, 1.0);

    /* you need to put a glTexCoord and glVertex call , one after the other,
         for each point */
    glBegin(GL_QUADS);
    glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, -1.0);
    glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, 1.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(1.0, 1.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(1.0, -1.0);
    glEnd();

    /* clean up */
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &gimp_image);

    glFlush();
}


/*
    north() - changes the title of window based on color selected
        if there exist problems with edges, perhaps overlay with thicker
        edged image with binary black or transparent colors.
*/
void north( unsigned char r, unsigned char g, unsigned char b){
    if (r == 255 && g == 0 && b == 0){
        glutSetWindowTitle("Algeria");}
    else if (r == 0 && g == 255 && b == 0){
        glutSetWindowTitle("Libya");}
    else if (r == 255 && g == 180 && b == 255){
        glutSetWindowTitle("Tunisia");}
    else if (r == 0 && g == 255 && b == 255){
        glutSetWindowTitle("Morocco");}
    else if (r == 255 && g == 220 && b == 140){
        glutSetWindowTitle("Western Sahara");}
    else if (r == 150 && g == 255 && b == 0){
        glutSetWindowTitle("Mauritania");}
    else if (r == 255 && g == 0 && b == 255){
        glutSetWindowTitle("Guinea-Bissau");}
    else if (r == 200 && g == 200 && b == 255){
        glutSetWindowTitle("Senegal");}
    else if (r == 255 && g == 255 && b == 0){
        glutSetWindowTitle("Gambia");}
    else if (r == 220 && g == 130 && b == 0){
        glutSetWindowTitle("Mali");}
    else if (r == 0 && g == 255 && b == 180){
        glutSetWindowTitle("Nigeria");}
    else if (r == 170 && g == 0 && b == 255){
        glutSetWindowTitle("Niger");}
    else if (r == 120 && g == 180 && b == 255){
        glutSetWindowTitle("Burkina Faso");}
    else if (r == 255 && g == 255 && b == 140){
        glutSetWindowTitle("Cote D'ivoire");}
    else if (r == 140 && g == 255 && b == 140){
        glutSetWindowTitle("Guinea");}
    else if (r == 170 && g == 40 && b == 170){
        glutSetWindowTitle("Liberia");}
    else if (r == 255 && g == 150 && b == 170){
        glutSetWindowTitle("Sierra Leone");}
    else if (r == 255 && g == 170 && b == 25){
        glutSetWindowTitle("Togo");}
    else if (r == 255 && g == 50 && b == 100){
        glutSetWindowTitle("Benin");}
    else if (r == 80 && g == 180 && b == 170){
        glutSetWindowTitle("Ghana");}
    else if (r == 255 && g == 115 && b == 180){
        glutSetWindowTitle("Chad");}
    else if (r == 160 && g == 180 && b == 255){
        glutSetWindowTitle("Egypt");}
    else if (r == 125 && g == 225 && b == 255){
        glutSetWindowTitle("Eritrea");}
    else if (r == 125 && g == 225 && b == 25){
        glutSetWindowTitle("Djibouti");}
    else if (r == 190 && g == 255 && b == 125){
        glutSetWindowTitle("Cabo Verde");}
    else if (r == 255 && g == 150 && b == 40){
        glutSetWindowTitle("Sudan");}

    // North/South Overlap
    else if (r == 230 && g == 220 && b == 100){
        glutSetWindowTitle("South Sudan");}
    else if (r == 255 && g == 215 && b == 115){
        glutSetWindowTitle("Cameroon");}
    else if (r == 235 && g == 60 && b == 60){
        glutSetWindowTitle("Ethiopia");}
    else if (r == 130 && g == 255 && b == 65){
        glutSetWindowTitle("Central African Republic");}
    else if (r == 175 && g == 110 && b == 255){
        glutSetWindowTitle("Somalia");}
    else if (r == 15 && g == 255 && b == 70){
        glutSetWindowTitle("Equatorial Guinea");}
    
    else if (r == 255 && g == 255 && b == 255){
        glutSetWindowTitle("Non-Political Area Selected");}
    // Otherwise indeterminable.
    else{glutSetWindowTitle("Indeterminable Political Area Selected");}
}

/*
    south() - changes the title of window based on color selected
        if there exist problems with edges, perhaps overlay with thicker
        edged image with binary black or transparent colors.
*/
void south( unsigned char r, unsigned char g, unsigned char b){
    if (r == 255 && g == 0 && b == 0){
        glutSetWindowTitle("Democratic Republic of the Congo");}
    else if (r == 0 && g == 255 && b == 0){
        glutSetWindowTitle("Kenya");}
    else if (r == 255 && g == 180 && b == 255){
        glutSetWindowTitle("Gabon");}
    else if (r == 0 && g == 255 && b == 255){
        glutSetWindowTitle("Congo Republic");}
    else if (r == 255 && g == 220 && b == 140){
        glutSetWindowTitle("Angola");}
    else if (r == 150 && g == 255 && b == 0){
        glutSetWindowTitle("Namibia");}
    else if (r == 255 && g == 0 && b == 255){
        glutSetWindowTitle("Tanzania");}
    else if (r == 200 && g == 200 && b == 255){
        glutSetWindowTitle("Uganda");}
    else if (r == 255 && g == 255 && b == 0){
        glutSetWindowTitle("Rwanda");}
    else if (r == 220 && g == 130 && b == 0){
        glutSetWindowTitle("Zambia");}
    else if (r == 0 && g == 255 && b == 180){
        glutSetWindowTitle("Malawi");}
    else if (r == 170 && g == 0 && b == 255){
        glutSetWindowTitle("Mozambique");}
    else if (r == 120 && g == 180 && b == 255){
        glutSetWindowTitle("Burundi");}
    else if (r == 255 && g == 255 && b == 140){
        glutSetWindowTitle("Zimbabwe");}
    else if (r == 140 && g == 255 && b == 140){
        glutSetWindowTitle("Swaziland");}
    else if (r == 170 && g == 40 && b == 170){
        glutSetWindowTitle("Botswana");}
    else if (r == 255 && g == 150 && b == 170){
        glutSetWindowTitle("South Africa");}
    else if (r == 255 && g == 170 && b == 25){
        glutSetWindowTitle("Madagascar");}
    else if (r == 255 && g == 50 && b == 100){
        glutSetWindowTitle("Comoros");}
    else if (r == 80 && g == 180 && b == 170){
        glutSetWindowTitle("Lesotho");}
    else if (r == 255 && g == 115 && b == 180){
        glutSetWindowTitle("Mauritius");}
    else if (r == 160 && g == 180 && b == 255){
        glutSetWindowTitle("Cabinda");}
    else if (r == 125 && g == 225 && b == 255){
        glutSetWindowTitle("Seychelles");}
    else if (r == 125 && g == 225 && b == 25){
        glutSetWindowTitle("Sao Tome and Principe");}

    // North/South Overlap
    else if (r == 230 && g == 220 && b == 100){
        glutSetWindowTitle("South Sudan");}
    else if (r == 255 && g == 215 && b == 115){
        glutSetWindowTitle("Cameroon");}
    else if (r == 235 && g == 60 && b == 60){
        glutSetWindowTitle("Ethiopia");}
    else if (r == 130 && g == 255 && b == 65){
        glutSetWindowTitle("Central African Republic");}
    else if (r == 175 && g == 110 && b == 255){
        glutSetWindowTitle("Somalia");}
    else if (r == 15 && g == 255 && b == 70){
        glutSetWindowTitle("Equatorial Guinea");}
    else if (r == 255 && g == 150 && b == 40){
        glutSetWindowTitle("Sudan");}

    else if (r == 255 && g == 255 && b == 255){
        glutSetWindowTitle("Non-Political Area Selected");}
    // Otherwise indeterminable.
    else{glutSetWindowTitle("Indeterminable Political Area Selected");}
}

int rand_lim(int limit) {
    int divisor = RAND_MAX/(limit+1);
    int retval;
    do { 
        retval = rand() / divisor;
    } while (retval > limit);
    return retval;
}

/*
    onClick() - Will find the color of the pixel selected and return valid
        output in title bar.
*/
void onClick(int button, int state, int x, int y){
    if ((button == GLUT_LEFT_BUTTON ) && (state == GLUT_DOWN)){
        unsigned char pixel[3];
        //printf("x: %d y: %d\n", x, y);
        //x = glutGet(GLUT_WINDOW_WIDTH) - x;
        // for some reason, had to invert the y coord. no idea why.
        y = glutGet(GLUT_WINDOW_HEIGHT) - y;
        glReadPixels(x , y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);
        //puts("Pixel:");
        //printf("r %d, g %d, b %d \n", pixel[0], pixel[1], pixel[2]);
        
        // Massive conditional for hardcoded values of colors in GLub.

        /* if black, get random pixel color within 3x3, 5x5, 7x7, then 9x9,             if continuously black. use new chosen pixel to determine if
            north or south, going to need 
        */
        
        if(pixel[0] <= 100 && pixel[1] <= 100 && pixel[2] <= 100){
            unsigned char tmp[3];
            int tmp_x, tmp_y, t1, t2;
            int searching = 1;
            for(int i = 3, k=1; i <= 17 && searching; i+=2,k++){
                tmp_x = x + rand_lim(i) - i; // random starting pixel
                tmp_y =  y + rand_lim(i) - i;
                t1 = 0;
                t2 = 0;
                for(int j = 0; j < i*i && searching; j++){
                    if (tmp_x+t1 >= i){
                        tmp_x = x - k;
                        t1 = 0;
                        t2++;
                    }
                    if (tmp_y+t2 >= i){
                        tmp_y = y - k;
                        t2 = 0;
                    }
                    if (tmp_x+t1 == x && tmp_y+t2 == y)
                        continue;

                    glReadPixels(tmp_x+t1, tmp_y+t2,
                        1, 1, GL_RGB, GL_UNSIGNED_BYTE, tmp);
                    if(tmp[0] > 100 || tmp[1] > 100 || tmp[2] > 100){
                        pixel[0] = tmp[0];
                        pixel[1] = tmp[1];
                        pixel[2] = tmp[2];
                        x = tmp_x+t1;
                        y = tmp_y+t2;
                        searching = 0;
                        //printf("reset x to %d and y to %d\n", x, y);
                        //printf("reset pix: r %d, g %d, b %d\n",
                        //    pixel[0], pixel[1], pixel[2]);
                    }
                    t1++; t2++;
                }
            }
        }
        
        // split up colors by south and south.
        if (y > (height/2) + (height/20) ){
            north(pixel[0],pixel[1],pixel[2]);
        }
        else{
            south(pixel[0], pixel[1], pixel[2]);
        }
    }
}

void keyPressed(unsigned char key, int x, int y){
    if (key == 27){
        exit(0);
    }
    else{
        glutSetWindowTitle("Africa Political Map");
    }
}

void main(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
    glutInitWindowSize(width, height);
    glutCreateWindow("Africa Political Map");
    
    initGL();
    
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutMouseFunc(onClick);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();
}
