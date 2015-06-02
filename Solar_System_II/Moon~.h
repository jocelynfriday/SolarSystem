//
//  Header.h
//  Solar_System_II
//
//  Created by Jocelyn FRIDAY on 25/11/2012.
//  Copyright (c) 2012 Jocelyn FRIDAY. All rights reserved.
//

#ifndef Solar_System_II_Moon_h
#define Solar_System_II_Moon_h

#include <GLUT/GLUT.h>
#include "SolarSphere.h"

//technically should be satelites
typedef struct Moon
{
    SolarSphere moonBody;
    GLfloat orbitAround;
    GLfloat locOfCenter; //Distance away from the planet
    GLfloat daysPerOrbit;
    GLfloat dayOfOrbit;
    GLfloat hoursPerDay;
    GLfloat currentHour;
    
    
    
}Moon;



#endif
