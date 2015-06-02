//
//  Particle.h
//  Solar_System_II
//
//  Created by Jocelyn FRIDAY on 25/11/2012.
//  Copyright (c) 2012 Jocelyn FRIDAY. All rights reserved.
//

#ifndef Solar_System_II_Particle_h
#define Solar_System_II_Particle_h

#include "SolarSphere.h"
#include <GLUT/GLUT.h>

//Using Newton's second law: mas* acceleration = sum of the force acting on a particle
//aceleration and force are vectors

typedef struct Particle
{
    SolarSphere particle; // has mass component
    enum bool alive;
    GLfloat position[3]; // x,y, z
    GLfloat velocity [3]; //= {dx, dy, dt};
    
}Particle;

#endif
