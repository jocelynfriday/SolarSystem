//
//  Planet.h
//  Solar_System_II
//
//  Created by Jocelyn FRIDAY on 25/11/2012.
//  Copyright (c) 2012 Jocelyn FRIDAY. All rights reserved.
//

#ifndef Solar_System_II_Planet_h
#define Solar_System_II_Planet_h

#include <GLUT/GLUT.h>
#include "SolarSphere.h"
#include "Moon.h"
#include "Rings.h"

enum bool
{
    false,
    true
};

typedef struct Planet
{
    SolarSphere planetBody;
    //decimal in earth years
    GLfloat daysPerYear;
    GLfloat dayOfYear;
    GLfloat hoursPerDay;
    GLfloat locOfCenter; //Distance away from the sun
    GLfloat currentHour;
    GLfloat inclination; //inclination to Sun's equator in degrees
    enum bool hasMoons;
    enum bool hasRings;
    Rings rings;
    Moon currentMoon;
    int numOfMoons;
    
    
}Planet;



#endif
