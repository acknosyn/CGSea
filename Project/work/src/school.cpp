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
#include "school.hpp"

using namespace std;
using namespace comp308;

School::School() {
	// init fish
	int i = 0;
	for (; i < fishAmount; i++) {
		vec3 pos = vec3(0, 1, 0);
		Fish fish = Fish(pos);

		schoolOfFish[i] = fish;
	}

	// place fish around scene
	initialisePositions();
}

void School::renderSchool() {
	schoolOfFish[0].renderFish();
	renderSphere();
}

void School::renderSphere() {
	glPushMatrix(); {
		glColor3f(0.3, 0.8, 0.3);
		glutWireSphere(sphereRadius, 50, 50);
	} glPopMatrix();
}

void School::initialisePositions() {
	// places fish randomly outside of the sphere
}