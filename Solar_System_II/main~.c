//
//  main.c
//  CG Solar System Practical
//
//  Copyright (c) 2012 090004718. All rights reserved.
//

#include <GLUT/GLUT.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Planet.h"
#include "Particle.h"
#include <time.h>
#include "SOIL.h"
#define ROTATION_MAX 360
#define PI 3.141592653589793238462643383279502884197169399375105820974944592
#define MAX_NUMBER_OF_PARTICLES 10

#define maxView 3


GLint windowWidth = 1800, windowHeight = 700;

/******** Trackball globals*******/
//Global values for trackball- like functionality.  Code modified off of code provided in above book.
GLfloat angle = 0.0, axis[3], trans[3];
enum bool trackingMouse = false;
enum bool redrawContinue = false;
enum bool trackingballMove = false;

//bool arrow = false;
int viewerPosition = 0;
GLfloat viewer[3] = {0.0, 0.0, 1.0};
GLfloat lastPosition[3] = {0.0F, 0.0F, 0.0F};
GLint currentX, currentY;
GLint startX, startY;

/********* Solar System Globals **********/

//Solar System Globals
Planet solarSystemObjects[9];
GLfloat increment = .1;

//Particle Simulation
Particle particles[MAX_NUMBER_OF_PARTICLES];
GLfloat lastTime = 0.0;
GLfloat presentTime = 0.0;
GLfloat numberOfParticles;
GLfloat speed = 1.0;
GLfloat coefficientOfRestitution = 1.0;
enum bool gravity = true;

/********* Lighting **********/
GLfloat lightPosition0[] = {0.0, 0.0, 0.0, 1.0};

GLfloat diffuse0[] = {1.0, 1.0, 1.0, 1.0};
GLfloat ambient0[] = {.01, .01, .01, 1.0};
GLfloat specular0[] = {1.0, 1.0, 1.0, 1.0};
//Sun's emission value
GLfloat emission[] = {0.81, 0.71, 0.23, 1.0};
GLfloat emmissionOff[] = {0.0,0.0,0.0,0.0};

//flags
enum bool light = true;
enum bool visablePathways = false;
enum bool move = false;

GLint view = 0;


void myInit()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.02);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.01);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.02);
    //  GLuint tempText;
    
    
    //Create Solar System - "radius" values are chosen to give rough representation with difference between planets.
    //Number of days per year, length time to rotate around sun, is accurate
    
    //sun
    solarSystemObjects[0].planetBody.radius = 2.0;
    solarSystemObjects[0].planetBody.currentMaterial = gold;
    solarSystemObjects[0].planetBody.mass = 6.0; //Arbitrary values to make graphics look "good" - roughly x3 of radius
    solarSystemObjects[0].planetBody.texture = SOIL_load_OGL_texture("sun.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[0].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[0].daysPerYear = 1.0;
    solarSystemObjects[0].locOfCenter = 0; // located at center
    solarSystemObjects[0].hoursPerDay =   1.0;
    
    solarSystemObjects[0].hasMoons = false;
    solarSystemObjects[0].numOfMoons = 0;
    
    solarSystemObjects[0].inclination = 0;
    solarSystemObjects[0].hasRings = false;
    
    //Mercury
    solarSystemObjects[1].planetBody.radius = 0.1;
    solarSystemObjects[1].planetBody.currentMaterial = chrome;
    solarSystemObjects[1].planetBody.mass = 0.3;
    solarSystemObjects[1].planetBody.texture = SOIL_load_OGL_texture("mercury.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[1].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[1].daysPerYear = 87.97;
    solarSystemObjects[1].locOfCenter = 3.1;
    solarSystemObjects[1].hoursPerDay = 58.65;
    
    solarSystemObjects[1].hasMoons = false;
    solarSystemObjects[1].numOfMoons = 0;
    
    solarSystemObjects[1].inclination = 3.38;
    solarSystemObjects[1].hasRings = false;
    
    
    
    //Venus
    solarSystemObjects[2].planetBody.radius = 0.2;
    solarSystemObjects[2].planetBody.currentMaterial = yellowPlastic;
    solarSystemObjects[2].planetBody.mass = 0.6;
    solarSystemObjects[2].planetBody.texture = SOIL_load_OGL_texture("venus.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[2].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[2].daysPerYear = 224.7;
    solarSystemObjects[2].locOfCenter = 3.9;
    solarSystemObjects[2].hoursPerDay = 243;
    
    solarSystemObjects[2].hasMoons = false;
    solarSystemObjects[2].numOfMoons = 0;
    
    solarSystemObjects[2].inclination = 3.86;
    solarSystemObjects[2].hasRings = false;
    
    
    //Earth
    solarSystemObjects[3].planetBody.radius = 0.23;
    solarSystemObjects[3].planetBody.currentMaterial = DKGreenCopper;
    solarSystemObjects[3].planetBody.mass = 0.69;
    solarSystemObjects[3].planetBody.texture = SOIL_load_OGL_texture("earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[3].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[3].daysPerYear = 365.26;
    solarSystemObjects[3].locOfCenter =  4.83;
    solarSystemObjects[3].hoursPerDay = 24;
    
    solarSystemObjects[3].hasMoons = true;
    solarSystemObjects[3].numOfMoons = 1;
    solarSystemObjects[3].currentMoon.moonBody.radius = 0.05;
    solarSystemObjects[3].currentMoon.moonBody.mass = 0.015;
    solarSystemObjects[3].currentMoon.moonBody.currentMaterial = pearl;
    solarSystemObjects[3].currentMoon.moonBody.texture = SOIL_load_OGL_texture("luner.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 == solarSystemObjects[3].currentMoon.moonBody.texture )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[3].currentMoon.locOfCenter =solarSystemObjects[3].planetBody.radius +  0.11; //orbit at a distance of .01 away from edge of planet
    solarSystemObjects[3].currentMoon.orbitAround = solarSystemObjects[3].locOfCenter; //orbit around Earth's center (locOfCenter)
    
    solarSystemObjects[3].currentMoon.daysPerOrbit = 27.332; //lenght of time to orbit the earth
    solarSystemObjects[3].currentMoon.hoursPerDay = 27.322; //From earth, can never see the far side of the moon as
    
    solarSystemObjects[3].inclination = 7.155;
    solarSystemObjects[3].hasRings = false;
    
    
    //Mars
    solarSystemObjects[4].planetBody.radius = 0.13;
    solarSystemObjects[4].planetBody.currentMaterial = bronze;
    solarSystemObjects[4].planetBody.mass = 0.39;
    solarSystemObjects[4].planetBody.texture = SOIL_load_OGL_texture("mars.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[4].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[4].daysPerYear = 686.98;
    solarSystemObjects[4].locOfCenter = 5.69;
    solarSystemObjects[4].hoursPerDay = 1.03;
    
    solarSystemObjects[4].hasMoons = false; //2 moons
    solarSystemObjects[4].numOfMoons = 0;
    
    solarSystemObjects[4].inclination = 5.65;
    solarSystemObjects[4].hasRings = false;
    
    //Jupiter
    solarSystemObjects[5].planetBody.radius = 0.5;
    solarSystemObjects[5].planetBody.currentMaterial = ruby;
    solarSystemObjects[5].planetBody.mass = 1.5;
    solarSystemObjects[5].planetBody.texture = SOIL_load_OGL_texture("jupiter.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[5].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[5].daysPerYear = 4332.0;
    //Farther distance apart to indicate astroid belt
    //Jovian year
    solarSystemObjects[5].locOfCenter = 7.32;
    //Jovian day
    solarSystemObjects[5].hoursPerDay = 9.8;
    
    solarSystemObjects[5].hasMoons = true;
    solarSystemObjects[5].numOfMoons = 1; // Really has 67 moons, not one
    solarSystemObjects[5].currentMoon.moonBody.radius = 0.05;
    solarSystemObjects[5].currentMoon.moonBody.mass = 0.015;
    solarSystemObjects[5].currentMoon.moonBody.currentMaterial = pearl;
    solarSystemObjects[5].currentMoon.moonBody.texture = SOIL_load_OGL_texture("luner.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 == solarSystemObjects[5].currentMoon.moonBody.texture )
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[5].currentMoon.locOfCenter = solarSystemObjects[5].planetBody.radius +  0.11; //orbit at a distance of .01 away from edge of planet
    solarSystemObjects[5].currentMoon.orbitAround = solarSystemObjects[5].locOfCenter; //orbit around jupiter's center (locOfCenter)
    
    solarSystemObjects[5].currentMoon.daysPerOrbit = 27.332; //general (same as earth) as not modeling a specific moon
    solarSystemObjects[5].currentMoon.hoursPerDay = 27.322; //same property as moon, because using this as a theoretic base
    
    solarSystemObjects[5].inclination = 6.09;
    solarSystemObjects[5].hasRings = true;
    solarSystemObjects[5].rings.innerRadius = (solarSystemObjects[5].planetBody.radius + 0.1);
    solarSystemObjects[5].rings.outerRadius = (solarSystemObjects[5].rings.innerRadius + 0.02);
    solarSystemObjects[5].rings.degreeOfRotation = 90;
    
    //Saturn
    solarSystemObjects[6].planetBody.radius = 0.42;
    solarSystemObjects[6].planetBody.currentMaterial = pearl;
    solarSystemObjects[6].planetBody.mass = 1.26;
    solarSystemObjects[6].planetBody.texture = SOIL_load_OGL_texture("saturn.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[6].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[6].daysPerYear = 10760.6;
    solarSystemObjects[6].locOfCenter = 8.74;
    solarSystemObjects[6].hoursPerDay = 10.2;
    
    solarSystemObjects[6].hasMoons = false; //Practical only calls for 2 moons, can easily add correct number of moons
    solarSystemObjects[6].numOfMoons = 0; //60 moons
    solarSystemObjects[6].inclination = 5.51;
    
    solarSystemObjects[6].hasRings = true;
    solarSystemObjects[6].rings.innerRadius = (solarSystemObjects[6].planetBody.radius + 0.1);
    solarSystemObjects[6].rings.outerRadius = (solarSystemObjects[6].rings.innerRadius + 0.15);
    solarSystemObjects[6].rings.degreeOfRotation = 110;
    
    
    
    //Uranus
    solarSystemObjects[7].planetBody.radius =0.3;
    solarSystemObjects[7].planetBody.currentMaterial = bluePearl;
    solarSystemObjects[7].planetBody.texture = SOIL_load_OGL_texture("uranus.jpg",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[7].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[7].daysPerYear = 30685.5;
    solarSystemObjects[7].locOfCenter = 9.96;
    solarSystemObjects[7].hoursPerDay = 17.9;
    
    solarSystemObjects[7].hasMoons = false; // 27 moon
    solarSystemObjects[7].numOfMoons = 0;
    
    solarSystemObjects[7].inclination = 6.48;
    
    solarSystemObjects[7].hasRings = true;
    solarSystemObjects[7].rings.innerRadius = (solarSystemObjects[7].planetBody.radius + 0.1);
    solarSystemObjects[7].rings.outerRadius = (solarSystemObjects[7].rings.innerRadius + 0.01);
    solarSystemObjects[7].rings.degreeOfRotation = 0;
    
    
    //Neptune
    solarSystemObjects[8].planetBody.radius = 0.295;
    solarSystemObjects[8].planetBody.currentMaterial = turquoise;
    solarSystemObjects[8].planetBody.mass = 0.885;
    solarSystemObjects[8].planetBody.texture = SOIL_load_OGL_texture("neptune.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_COMPRESS_TO_DXT);
    if( 0 ==  solarSystemObjects[8].planetBody.texture)
    {
        printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
    }
    solarSystemObjects[8].daysPerYear = 60194.8;
    solarSystemObjects[8].locOfCenter = 11.055;
    solarSystemObjects[8].hoursPerDay = 6.39;
    
    solarSystemObjects[8].hasMoons = false; //13 moons
    solarSystemObjects[8].numOfMoons = 0;
    
    solarSystemObjects[8].inclination = 6.43;
    
    solarSystemObjects[8].hasRings = true;
    solarSystemObjects[8].rings.innerRadius = (solarSystemObjects[8].planetBody.radius + 0.1);
    solarSystemObjects[8].rings.outerRadius = (solarSystemObjects[8].rings.innerRadius + 0.01);
    solarSystemObjects[8].rings.degreeOfRotation = 90;
    
    //taken from http://www.lonesock.net/soil.html
    /* load 6 images into a new OpenGL cube map, forcing RGB */
    /* GLuint tex_cube = SOIL_load_OGL_cubemap("images/Skybox/xf.jpg", "images/Skybox/xp.jpg","images/Skybox/yf.jpg", "images/Skybox/yp.jpg", "images/Skybox/zf.jpg", "images/Skybox/zp.jpg", SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
     
     */
    
    //Set  current day and hour to 0
    for (int i = 0; i < 9; i ++)
    {
        solarSystemObjects[i].dayOfYear = 0;
        solarSystemObjects[i].currentHour = 0;
        
        if (solarSystemObjects[i].hasMoons)
        {
            //set lunar days and hours to 0 if moons exist
            for(int j = 0; j < solarSystemObjects[i].numOfMoons; j ++)
            {
                solarSystemObjects[i].currentMoon.dayOfOrbit = 0;
                solarSystemObjects[i].currentMoon.currentHour = 0;
            }
        }
    }
    
    // initialize particles
    for(int a = 0; a < numberOfParticles; a++)
    {
        particles[a].particle.mass = .1;
        particles[a].particle.currentMaterial = yellowPlastic;
        particles[a].particle.radius = .01;
        
        for(int b = 0; b < 3; b++)
        {
            particles[a].position[b] = 2.0*((float) random()/ RAND_MAX) - 1.0;
            particles[a].velocity[b] = speed*2.0*((float)random() / RAND_MAX) - 1.0;
        }
    }
        
}

/********************** Rendering Functions ***************************/


void myReshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w,(GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(39, (GLfloat)w /(GLfloat) h, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    gluLookAt(-1.0, 2.0, 13.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //Update global variables
    windowHeight = h;
    windowWidth = w;
}

void changeView(int viewNumber)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    switch(viewNumber)
    {
        case 0:// default
            gluLookAt(-1.0, 2.0, 13.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
            break;
            
        case 1:
            gluLookAt(-1.0, 2.0, 13.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
            //camz = 13.0, camy = 2.0, camx = -1.0
            //up x = 0.0, up z = 1.0, upy = 0.0
            break;
            
        case 2:
            gluLookAt(0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.0 , 1.0);
            //camz = 0.0, camy = 15.0, camx = 0.0
            //up x = 0.0, up z = 0.0, upy = 1.0
            break;
            
        case 3:
            gluLookAt(-1.0, -2.0, -13.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
            break;
    }
}



void drawPathways()
{
    if(visablePathways)
    {
        glDisable(GL_LIGHTING);
        
        for (int i = 1; i < 9; i ++)
        {
            glColor3f(1.0, 1.0, 1.0);
            
            //drawing orbitals of planets
            glBegin(GL_LINE_LOOP);
            for (int j = 0 ; j < 360; j ++)
            {
                float angle = j * (2.0* PI/180);
                float x = cos(angle)*solarSystemObjects[i].locOfCenter;
                float z = sin(angle)*solarSystemObjects[i].locOfCenter;
                glVertex3f(x, 0, z);
                
            }
            glEnd();
            
        }
        
        if(light)
        {
            glEnable(GL_LIGHTING);
            glColor3f(0.0, 0.0, 0.0);
        }
    }
}

void collision (int n)
{
    for (int i = 0; i < 3; i++)
    {
        if(particles[n].position[i] >= 1.0)
        {
            particles[n].velocity[i] = -coefficientOfRestitution*particles[n].velocity[i];
            particles[n].position[i] = 1.0 - coefficientOfRestitution*(particles[n].position[i] - 1.0);
        }
        
        if(particles[n].position[i] <= -1.0)
        {
            particles[n].velocity[i] = -coefficientOfRestitution*particles[n].velocity[i];
            particles[n].position[i] = -1.0 - coefficientOfRestitution*(particles[n].position[i] + 1.0);
        }
    }
}

GLfloat forces(GLint i, GLint j)
{
    if(!gravity)
        return 0.0;
    else if (j == 1)
        return -1.0;
    else return 0.0;
}

void myDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if(!light)
        glColor3f(1.0, 1.0, 1.0);
    else
        glColor3f(0.0, 0.0, 0.0);
    
    //trackball motion
    if(trackingballMove)
    {
        glRotatef(angle, axis[0], axis[1], axis[2]);
    }
    
    
    GLUquadricObj *temp =  gluNewQuadric();
    gluQuadricNormals(temp, GLU_SMOOTH);
    gluQuadricTexture(temp, GLU_TRUE);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

       
    if(visablePathways)
        drawPathways();
    
    
    for (int i = 0; i < 9; i ++)
    {
        glPushMatrix();
        
        //draw moons if they exist
        
        
        
        //Declare material properties of each planet
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,solarSystemObjects[i].planetBody.currentMaterial.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,solarSystemObjects[i].planetBody.currentMaterial.diffuse);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,solarSystemObjects[i].planetBody.currentMaterial.shineness *128.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, solarSystemObjects[i].planetBody.currentMaterial.specular);
        glColor3fv(solarSystemObjects[i].planetBody.currentMaterial.color);
        
        //Rotate around sun
        glRotatef(360*solarSystemObjects[i].dayOfYear / solarSystemObjects[i].daysPerYear, 0.0, 1.0, 0.0); //This will self-rotate the sun
        //translate on axis
        glTranslatef(solarSystemObjects[i].locOfCenter, 0, 0);
        
        if (solarSystemObjects[i].hasRings)
        {
            glDisable(GL_LIGHTING);
            glRotated(solarSystemObjects[i].rings.degreeOfRotation, 1, 0, 0);
            gluDisk(temp, (solarSystemObjects[i].rings.innerRadius), solarSystemObjects[i].rings.outerRadius, 300, 300);
            if(light)
            {
                glEnable(GL_LIGHTING);
            }
            glRotated(-solarSystemObjects[i].rings.degreeOfRotation, 1, 0, 0);
            
        }
        //rotate around self
        glRotatef(360*solarSystemObjects[i].currentHour/solarSystemObjects[i].hoursPerDay, 0, 1, 0);
        
        glRotatef(90, 1, 0, 0);
        //allow the sun to have an emmissive quality
        if(i == 0)
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
        
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, solarSystemObjects[i].planetBody.texture);
        gluSphere(temp, solarSystemObjects[i].planetBody.radius, 90, 50);
        glDisable(GL_TEXTURE_2D);
       
        //turn off the emissive quality and reasjust the position of light0
        if(i == 0)
        {
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmissionOff);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
        }
        
        if(solarSystemObjects[i].hasMoons)
        {
            for(int j = 0; j < solarSystemObjects[i].numOfMoons; j ++)
            {
                if (visablePathways)
                {
                    glDisable(GL_LIGHTING);
                    
                    glColor3f(1.0, 1.0, 1.0);
                    
                    //drawing orbitals of moons
                    glBegin(GL_LINE_LOOP);
                    for (int k = 0 ; k < 360; k ++)
                    {
                        float angle = k * (2.0* PI/180);
                        float x = cos(angle)*(solarSystemObjects[i].currentMoon.locOfCenter);
                        float z = sin(angle)*(solarSystemObjects[i].currentMoon.locOfCenter);
                        glVertex3f(x, 0, z);
                        
                    }
                    glEnd();
                    
                    
                    
                    if(light)
                    {
                        glEnable(GL_LIGHTING);
                        glColor3f(0.0, 0.0, 0.0);
                    }
                }// close pathways for loop
                
                
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,solarSystemObjects[i].currentMoon.moonBody.currentMaterial.ambient);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,solarSystemObjects[i].currentMoon.moonBody.currentMaterial.diffuse);
                glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,solarSystemObjects[i].currentMoon.moonBody.currentMaterial.shineness * 128.0);
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, solarSystemObjects[i].currentMoon.moonBody.currentMaterial.specular);
                glColor3fv(solarSystemObjects[i].currentMoon.moonBody.currentMaterial.color);
                
                glRotatef( 360.0*solarSystemObjects[i].currentMoon.dayOfOrbit/solarSystemObjects[i].currentMoon.daysPerOrbit, 1.0, 0.0, 0.0);
                glTranslatef(solarSystemObjects[i].currentMoon.locOfCenter, 0, 0);
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, solarSystemObjects[i].currentMoon.moonBody.texture);
                gluSphere(temp, solarSystemObjects[i].currentMoon.moonBody.radius, 20, 20);
                glDisable(GL_TEXTURE_2D);
            }// close for loops of moons
        }// close if moons
        
        
        glPopMatrix();
    }
    glBegin(GL_POINTS);
    for (int p = 0; p < numberOfParticles; p ++)
    {
        glPushMatrix();
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,particles[p].particle.currentMaterial.ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,particles[p].particle.currentMaterial.diffuse);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,particles[p].particle.currentMaterial.shineness * 128.0);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, particles[p].particle.currentMaterial.specular);
        glColor3fv(particles[p].particle.currentMaterial.color);
        glTranslatef(particles[p].position[0], particles[p].position[1], particles[p].position[2]);
        gluSphere(temp, particles[p].particle.radius, 5, 5);
        glPopMatrix();
    }
    glEnd();
    glutSwapBuffers();
}



static void animate()
{
    if(move)
    {
        //increment days + hours on moons and planets
        for(int i = 1; i < 9; i ++)
        {
            solarSystemObjects[i].dayOfYear += increment;
            solarSystemObjects[i].currentHour += .1;
            if (solarSystemObjects[i].dayOfYear > solarSystemObjects[i].daysPerYear)
            {
                solarSystemObjects[i].dayOfYear = 0;
            }
            
            if(solarSystemObjects[i].currentHour > solarSystemObjects[i].hoursPerDay)
                solarSystemObjects[i].currentHour = 0;
            
            if (solarSystemObjects[i].hasMoons)
            {
                for (int j = 0; j < solarSystemObjects[i].numOfMoons; j++)
                {
                    solarSystemObjects[i].currentMoon.dayOfOrbit += increment;
                    solarSystemObjects[i].currentMoon.currentHour += .1;
                }
                if (solarSystemObjects[i].currentMoon.currentHour > solarSystemObjects[i].currentMoon.hoursPerDay)
                {
                    solarSystemObjects[i].currentMoon.currentHour = 0;
                }
            }
            
        }
        
        //move particles
        GLfloat dt;
        presentTime = glutGet(GLUT_ELAPSED_TIME); // in milliseconds
        dt = 0.001*(presentTime - lastTime); // in seconds
        for (int p = 0; p < numberOfParticles; p ++)
        {
            for(int q = 0; q < 3; q ++)
            {
                particles[p].position[q] += dt*particles[p].velocity[q];
                particles[p].velocity[q] += dt*forces(p,q)/particles[p].particle.mass;
            }
            collision(p);
        }
        lastTime = presentTime;
        glutPostRedisplay();
    }
}

/********************** User Interaction ***************************/


void myKeyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
            // ESC
        case 27:
            exit(0);
            break;
            
            //Space bar - planets move
        case 32:
            move = !move;
            break;
            
            // l or L - Light source come on/off
        case 108: case 76:
            light = !light;
            if(light)
                glEnable(GL_LIGHTING);
            else glDisable(GL_LIGHTING);
            break;
            
            //r or R - pathWays of planets displayed
        case 82: case 114:
            visablePathways = !visablePathways;
            break;
            
            //v or V - Cycle through predefined view points
        case 118: case 86:
            view ++;
            if (view > maxView)
            {
                view = 0;
            }
            changeView(view);
            break;
            
            //s or S - Change between texture and shading
        case 83: case 115:
            break;
            
            // < Decrease speed
        case 60:
            if (increment > .1)
            {
                increment --;
            }
            break;
            
            // > increase speed
        case 62:
            increment ++;
            break;
            
    }
    glutPostRedisplay();
}

/********************** Trackball Functions ***************************/
//Trackball code reused from previous CG practical
void pivot(int x, int y, int w, int h, GLfloat v[3])
{
    
    float d, a;
    
    // project x,y onto a hemipser centered within specified width and height
    v[0] = (2.0F*x - w)/w;
    v[1] = (h - 2.0F*y)/h;
    d = (float)sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float)cos((M_PI/2.0F) * ((d < 1.0F) ? d: 1.0F));
    a = 1.0F/ (float) (sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2])); // 1/ pythagorian triangle
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
}

void mouseMotion(int x, int y)
{
    GLfloat currentPosition[3], dx, dy, dz;
    pivot(x, y, windowWidth, windowHeight, currentPosition);
    if(trackingMouse)
    {
        dx = currentPosition[0] - lastPosition[0];
        dy = currentPosition[1] - lastPosition[1];
        dz = currentPosition[2] - lastPosition[2];
        
        if(dx || dy || dz)
        {
            angle = 90.0F *sqrt(dx*dx + dy*dy + dz*dz);
            
            axis[0] = lastPosition[1]*currentPosition[2] - lastPosition[2]*currentPosition[1];
            axis[1] = lastPosition[2]*currentPosition[0] - lastPosition[0]*currentPosition[2];
            axis[2] = lastPosition[0]*currentPosition[1] - lastPosition[1]*currentPosition[0];
            
            //update last possition to current position across the board
            lastPosition[0] = currentPosition[0];
            lastPosition[1] = currentPosition[1];
            lastPosition[2] = currentPosition[2];
        }
    }
    //  glutPostRedisplay();
}

void stopMotion(int x, int y)
{
    trackingMouse = false;
    if(startX != x || startY != y)
    {
        redrawContinue = true;
    }
    else
    {
        angle = 0.0F;
        redrawContinue = false;
        trackingballMove = false;
    }
}

void startMotion(int x, int y)
{
    trackingMouse = true;
    redrawContinue = false;
    startX = x;
    startY = y;
    currentX = x;
    currentY = y;
    pivot(x, y, windowWidth, windowHeight, lastPosition);
    trackingballMove = true;
    
}

void myMouse(int btn, int state, int x, int y)
{
    if(btn == GLUT_LEFT_BUTTON)
    {
        switch(state)
        {
            case GLUT_DOWN:
                y = windowHeight - y; // adjust coordinates
                startMotion(x, y);
                break;
            case GLUT_UP:
                stopMotion(x, y);
                break;
        }
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Solar System");
    
    myInit();
    
    glutIdleFunc(animate);
    glutReshapeFunc(myReshape);
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutMotionFunc(mouseMotion);
    glutMainLoop();
    
}

