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

class School {
private:
	int fishAmount = 10;
	std::vector<Fish> schoolOfFish;

public:
	School();

	float sphereRadius = 20.0;
	bool step = false;

	void update(bool); // run every frame

	void renderSchool();
	void renderSphere();
	void renderCentreMass();

	void initialisePositions();

	void moveAllFishToNewPositions();
	comp308::vec3 rule1(Fish *);
	comp308::vec3 rule2(Fish *);
	comp308::vec3 rule3(Fish *);

	bool isBoundsCollided(Fish);
	void moveFishToOppositeOfBounds(Fish *);
};