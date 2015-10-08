//---------------------------------------------------------------------------
//
// Francesco Badraun 2015
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "fish.hpp"

using namespace std;
using namespace comp308;

Fish::Fish() {
	position = vec3(0, 1, 0);
	velocity = vec3(0, 0, 0);
}

void Fish::renderFish() {

	glPushMatrix(); {
		// colour - light blue
		glColor3f(0.3, 0.4, 0.8);

		// orient fish to it's position
		float angle = degrees(acos(dot(vec3::k(), position)));
		vec3 axis = cross(vec3::k(), position);

		glRotatef(angle, axis.x, axis.y, axis.z);

		// render geometry
		glutSolidCone(0.5, 1, 6, 5);
	}
	glPopMatrix();
}
