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
#include <cstdlib> // for rand and srand
#include <ctime> // for time

#include "comp308.hpp"
#include "school.hpp"

using namespace std;
using namespace comp308;

School::School() {
	// init fish
	int i = 0;
	for (; i < fishAmount; i++) {
		Fish fish = Fish();

		schoolOfFish.push_back(fish);
	}

	// place fish around scene
	srand (static_cast <unsigned> (time(0))); // seed random number
	initialisePositions();
}

void School::renderSchool() {
	for(std::vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
    	it->renderFish();
    	//cout<<it->getPos().x<<" "<<it->getPos().y<<" "<<it->getPos().z<<"\n"<<endl;
	}
	
	renderSphere();
}

void School::renderSphere() {
	glPushMatrix(); {
		glColor3f(0.3, 0.8, 0.3);
		glutWireSphere(sphereRadius, 50, 50);
	} glPopMatrix();
}

void School::initialisePositions() {
	// places fish randomly outside of the sphere (Within a small distance)

	// generate random x,y,z values just outside the sphere
	float high = 1.0 + sphereRadius;
	float low = -high;

	for(std::vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {

		float x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float z = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));

		// create new position vector
		vec3 newPos = vec3(x, y, z);

		newPos /= length(newPos); // normalised

		newPos *= sphereRadius;

	    it->setPosition(newPos);

	    newPos *= 0.01;
	    it->setVelocity(-newPos);
	}
}