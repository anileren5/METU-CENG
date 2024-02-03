#include "vectorOps.h"
void setNthComponent(Vec4 &vector, int n, double value) {
    switch (n) {
        case 0:
            vector.x = value;
            break;
        case 1:
            vector.y = value;
            break;
        case 2:
            vector.z = value;
            break;
        case 3:
            vector.t = value;
            break;
        // Add handling for invalid values of n if necessary
    }
}


void perspectiveDivideVector4(Vec4& vector){
	if(vector.t != 1 && vector.t != 0) 
	{
		vector.x = vector.x/vector.t; 
		vector.y = vector.y/vector.t; 
		vector.z = vector.z/vector.t; 
		vector.t = 1;
	}
}