//---------------------------------------------------------------------------
//
// Francesco Badraun 2015
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"
#include "fish.hpp"
#include "geometry.hpp"

class School {
private:
	int fishAmount = 60;
	std::vector<Fish> schoolOfFish;
	bool info = false;
	Geometry * spongebob = nullptr;

public:
	School(Geometry * g);

	float boundsRadius = 20.0;
	bool step = false;

	void update(bool, bool); // run every frame

	void renderSchool();
	void renderBounds();

	void initialisePositions();

	void moveAllFishToNewPositions();
	comp308::vec3 rule1(Fish *);
	comp308::vec3 rule2(Fish *);
	comp308::vec3 rule3(Fish *);
	comp308::vec3 boundPosition(Fish *);
	comp308::vec3 avoidCoral(Fish *);
	
	void limitVelocity(Fish *);
	bool detectCoral(Fish *);
};