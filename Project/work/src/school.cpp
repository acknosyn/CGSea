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
#include "fish.hpp"

using namespace std;
using namespace comp308;

School::School() {
	// init fish
	int i = 0;
	for (; i < fishAmount; i++) {
		Fish fish = Fish();

		schoolOfFish.push_back(fish);
	}

	srand (static_cast <unsigned> (time(0))); // seed random number
	initialisePositions(); // place fish around scene
}

void School::update(bool play) {
	renderSchool();

	if (step) {
		moveAllFishToNewPositions();
		step = false;
	} else if (play) {
		moveAllFishToNewPositions();
	}
}

void School::renderSchool() {
	// render every fish
	for(std::vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
    	it->renderFish();
	}
	
	// render bounds
	renderSphere();
}

void School::renderSphere() {
	glPushMatrix(); {
		glColor4f(0.3, 0.8, 0.3, 0.5); // transparent green
		glutWireSphere(sphereRadius, 50, 50);
	} glPopMatrix();
}

void School::initialisePositions() {
	// places fish randomly on the surface of the sphere

	// generate random x,y,z values just outside the sphere
	float high = sphereRadius;
	float low = -high;

	for(std::vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {

		float x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float z = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));

		// create new position vector
		vec3 newPos = vec3(x, y, z);

		newPos /= length(newPos); // normalised

		newPos *= sphereRadius + schoolOfFish.front().fishLength;

	    it->setPosition(newPos);

	    newPos *= 0.01;
	    it->setVelocity(-newPos);
	}
}

void School::moveAllFishToNewPositions() {
	// move every fish's position by velocity
	for (std::vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {		
		vec3 pos = it->getPosition();
		vec3 vel = it->getVelocity();

		pos += vel;

		it->setPosition(pos);
		it->setVelocity(vel);

		// move fish to opposite side of bounds if it goes past bounds
		if (isBoundsCollided(*it)) {
			moveFishToOppositeOfBounds(&(*it)); // &(*it) gives function an address ('&') to the dereferenced pointer ('(*it)') which is a pointer
		}
	}
}

bool School::isBoundsCollided(Fish fish) {
	vec3 position = fish.getPosition();
	vec3 origin = vec3(0, 0, 0);

	vec3 distanceVector = position - origin;
	float distance = length(distanceVector);

	// how far is the fish allowed to go from the origin(center)
	float allowance = sphereRadius + fish.fishLength + 0.001; // 0.001 is so it doesn't collide when in its initialisePositions() position

	if (distance >= allowance) {
		return true;
	}

	return false;
}

void School::moveFishToOppositeOfBounds(Fish* fish) {
	fish->setPosition(-fish->getPosition()); // negative position is mirrored position (opposite)
}
