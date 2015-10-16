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
#include "geometry.hpp"

using namespace std;
using namespace comp308;

School::School(Geometry * g) {
	spongebob = g;
	// init fish
	int i = 0;
	for (; i < fishAmount; i++) {
		Fish fish = Fish();

		schoolOfFish.push_back(fish);
	}

	srand (static_cast <unsigned> (time(0))); // seed random number
	initialisePositions(); // place fish around scene
}

void School::update(bool play, bool i) {
	renderSchool();

	if (step) {
		moveAllFishToNewPositions();
		step = false;
	} else if (play) {
		moveAllFishToNewPositions();
	}

	info = i;
}

void School::renderSchool() {
	// render every fish
	for(vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
		if (it == schoolOfFish.begin()) {
			it->renderFish(info, spongebob, true);
		} else {
			it->renderFish(info, spongebob, false);
		}
	}
	
	if (info) {
		renderBounds();
	}
}

void School::renderBounds() {
	glPushMatrix(); {
		glColor4f(0.3, 0.4, 0.8, 0.5); // transparent blue
		
		glScalef(2.5, 1, 2.5);
		glutWireCube(boundsRadius * 2);
	} glPopMatrix();


	// coral bounds
	glPushMatrix(); {
		glColor3f(0.9, 0.2, 0.2);
		glTranslatef(0.0f,-18.0f,0.0f);
		glScalef(1.6, 1.4, 1.6);
		glutWireCube(10);
	} glPopMatrix();
}

void School::initialisePositions() {
	// places fish randomly on the surface of the sphere

	// generate random x,y,z values just outside the sphere
	float high = boundsRadius;
	float low = -high;

	for(vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {

		float x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));
		float z = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high - low)));

		// create new position vector
		vec3 newPos = vec3(x, y, z);

		newPos /= length(newPos); // normalised

		newPos *= boundsRadius + schoolOfFish.front().fishLength;

	    it->setPosition(newPos);

	    newPos *= 0.01;
	    it->setVelocity(-newPos);
	}
}
/*
	Actual boids algorithm
*/
void School::moveAllFishToNewPositions() {

	vec3 v1, v2, v3; // the 3 main rules for a boid
	vec3 v4, v5;

	for (vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {

		Fish *fish = &(*it); // &(*it) is an address ('&') to the dereferenced pointer ('(*it)'), which is a pointer

		v1 = rule1(fish);
		v2 = rule2(fish);
		v3 = rule3(fish);
		v4 = boundPosition(fish);
		v5 = avoidCoral(fish);

		vec3 velocity = fish->getVelocity() + v1 + v2 + v3 + v4 + v5;
		fish->setVelocity(velocity);

		limitVelocity(fish);

		vec3 position = fish->getPosition() + fish->getVelocity();
		fish->setPosition(position);
	}
}

/*
	Cohesion

	Rule 1: Boids try to fly towards the centre of mass of neighbouring boids.
	
	This uses the 'perceived centre' which is the centre of all the other fish, not including itself.
*/
vec3 School::rule1(Fish *fj) {

	vec3 pcj; // perceived centre, (centre of every fish not including fj)

	for (vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
		Fish *f = &(*it);

		if (f != fj) {
			pcj = pcj + f->getPosition();
		}
	}

	pcj = pcj / (schoolOfFish.size() - 1);

	return (pcj - fj->getPosition()) / 1000; // gives a vector which moves fish a percentage of the way towards the centre
}

/*
	Separation

	Rule 2: Boids try to keep a small distance away from other objects (including other boids).
*/
vec3 School::rule2(Fish *fj) {
	float minDistance = 1.5;

	vec3 c = vec3();

	for (vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
		Fish *f = &(*it);

		if (f != fj) {
			vec3 distanceBetweenFish = f->getPosition() - fj->getPosition();

			if (length(distanceBetweenFish) < minDistance) {
				c = c - distanceBetweenFish;
			}
		}
	}

	//cout << length(fj->getVelocity()) <<", " << length(c / 10.0) << endl;

	return c / 50.0; // lessen the amount of influence the vector has
}

/*
	Alignment

	Rule 3: Boids try to match velocity with near boids.

	Similar to rule 1, this uses the 'perceived velocity' which is the average velocity of all the other fish, not including itself.
*/
vec3 School::rule3(Fish *fj) {

	vec3 pvj; // perceived velocity, (velocity of every fish not including fj)

	for (vector<Fish>::iterator it = schoolOfFish.begin(); it != schoolOfFish.end(); ++it) {
		Fish *f = &(*it);

		if (f != fj) {
			pvj = pvj + f->getVelocity();
		}
	}

	pvj = pvj / (schoolOfFish.size() - 1);

	return (pvj - fj->getVelocity()) / 8;
}

comp308::vec3 School::boundPosition(Fish *f) {

	float x_min = -boundsRadius * 2.5;
	float y_min = -boundsRadius;
	float z_min = -boundsRadius * 2.5; 
	float x_max = boundsRadius * 2.5;
	float y_max = boundsRadius;
	float z_max = boundsRadius * 2.5;

	float am = 0.05; // amount

	vec3 v;
	vec3 pos = f->getPosition();

	if (pos.x < x_min) {
		v.x = am;
	}
	else if (pos.x > x_max) {
		v.x = -am;
	}
	if (pos.y < y_min) {
		v.y = am;
	}
	else if (pos.y > y_max) {
		v.y = -am;
	}
	if (pos.z < z_min) {
		v.z = am;
	}
	else if (pos.z > z_max) {
		v.z = -am;
	}

	return v;
}

comp308::vec3 School::avoidCoral(Fish *f) {
	float am = 0.05; // amount

	vec3 v;

	if (detectCoral(f)) {
		vec3 vel = f->getVelocity();

		//vel /= length(vel); // normalise

		//vel *= am;

		//v = vel;

		vec3 pos = f->getPosition();
		float f_size = 1.5f;

		vec3 cor = vec3(0.0f,-18.0f,0.0f);
		vec3 cor_size = vec3(8.0, 7.0, 8.0);


		if(pos.x > (cor.x - cor_size.x)) {
			v.x = -am;
		} else if (pos.x < (cor.x + cor_size.x)) {
			v.x = am;
		}

		if (pos.y < (cor.y + cor_size.y)) {
			v.y = am;
		}

		if(pos.z > (cor.z - cor_size.z)) {
			v.z = -am;
		} else if (pos.z < (cor.z + cor_size.z)) {
			v.z = am;
		}
	}

	return v;
}

void School::limitVelocity(Fish *f) {

	float velocityLimit = 0.5;
	
	vec3 velocity = f->getVelocity();

	if (length(velocity) > velocityLimit) {
		velocity = (velocity / length(velocity)) * velocityLimit;
		f->setVelocity(velocity);
	}
}

bool School::detectCoral(Fish *f) {
	
	vec3 pos = f->getPosition();
	float f_size = 1.5f;

	vec3 cor = vec3(0.0f,-18.0f,0.0f);
	vec3 cor_size = vec3(8.0, 7.0, 8.0);

	 //check the X axis
   if(abs(pos.x - cor.x) < f_size + cor_size.x)
   {
      //check the Y axis
      if(abs(pos.y - cor.y) < f_size + cor_size.y)
      {
          //check the Z axis
          if(abs(pos.z - cor.z) < f_size + cor_size.z)
          {
             return true;
          }
      }
   }

   return false;
}