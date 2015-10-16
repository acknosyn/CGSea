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
#include "geometry.hpp"

using namespace std;
using namespace comp308;

Fish::Fish() {
	position = vec3(0, 0, 0);
	velocity = vec3(0, 0, 0);
}

void Fish::renderFish(bool info, Geometry * geometry) {

	glPushMatrix(); {
		// translate to position of fish
		glTranslatef(position.x, position.y, position.z);

		// orient fish in direction of velocity
		float angle = degrees(acos(dot(vec3::k(), normalize(velocity))));
		vec3 axis = cross(vec3::k(), normalize(velocity));
		axis = normalize(axis);

		glRotatef(angle, axis.x, axis.y, axis.z);

		if (info) {
			// velocity vector
			glPushMatrix(); {
				glColor3f(0.9, 0.3, 0.3); // light red

				GLUquadricObj *quadObj = gluNewQuadric();
				gluCylinder(quadObj, 0.03, 0.03, length(velocity) + fishLength, 10, 10);
			} 
			glPopMatrix();
		}
		
		// render geometry
		glColor3f(0.2, 0.2, 0.6); // light green

		glScalef(0.2, 1, 1);
		float tailLength = fishLength * (1.0f/3.0f);
		glutSolidCone(0.3, tailLength, 6, 5);

		glScalef(1, 0.5, 1);
		float bodyLength = fishLength * (2.0f / 3.0f);
		glTranslatef(0, 0, tailLength + (bodyLength / 2));
		glutSolidSphere(bodyLength/2, 10, 10);
	}
	glPopMatrix();

	glPushMatrix(); {
		// translate to position of fish
		glTranslatef(position.x, position.y, position.z);
		// orient fish in direction of velocity
		float angle = degrees(acos(dot(vec3::k(), normalize(velocity))));
		vec3 axis = cross(vec3::k(), normalize(velocity));
		axis = normalize(axis);

		glRotatef(angle, axis.x, axis.y, axis.z);
		glRotatef(90, 1, 0, 0);

		glScalef(0.15, 0.15, 0.15);

		glEnable(GL_TEXTURE_2D);

		geometry->renderGeometry();
	} glPopMatrix();
}

vec3 Fish::getPosition() {
	return position;
}

vec3 Fish::getVelocity() {
	return velocity;
}

void Fish::setPosition(vec3 pos) {
	position = pos;
}

void Fish::setVelocity(vec3 vel) {
	velocity = vel;
}