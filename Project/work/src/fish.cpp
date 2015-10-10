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
#include "school.hpp"

using namespace std;
using namespace comp308;

Fish::Fish() {
	position = vec3(0, 0, 0);
	velocity = vec3(0, 0, 0);
}

void Fish::renderFish() {

	glPushMatrix(); {
		//velocity -= vec3( .001,.001,.001);
		//position += velocity;

		// translate to position of fish
		glTranslatef(position.x, position.y, position.z);

		// orient fish in direction of velocity
		float angle = degrees(acos(dot(vec3::k(), normalize(velocity))));
		vec3 axis = cross(vec3::k(), normalize(velocity));
		axis = normalize(axis);

		glRotatef(angle, axis.x, axis.y, axis.z);

		// velocity vector arrow
		glPushMatrix(); {
			glColor3f(0.9, 0.3, 0.3); // light red

			GLUquadricObj *quadObj = gluNewQuadric();
			gluCylinder(quadObj, 0.03, 0.03, length(velocity) + fishLength, 10, 10);
		} 
		glPopMatrix();

		// render geometry
		glColor3f(0.3, 0.4, 0.8); // light blue
		glutSolidCone(0.5, fishLength, 6, 5);
	}
	glPopMatrix();
}

void Fish::setPosition(vec3 pos) {
	position = pos;
}

void Fish::setVelocity(vec3 vel) {
	velocity = vel;
}

vec3 Fish::getPos() {
	return position;
}