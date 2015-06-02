//
//  SolarBody.h
//  Solar_System_II
//
//  Created by Jocelyn FRIDAY on 25/11/2012.
//  Copyright (c) 2012 Jocelyn FRIDAY. All rights reserved.
//

#ifndef Solar_System_II_SolarShere_h
#define Solar_System_II_SolarShere_h

#include <GLUT/GLUT.h>
#include "Material.h"

typedef struct solarSphere
{
    GLfloat mass;
    GLfloat radius;
    Material currentMaterial;
    
}SolarSphere;


#endif
