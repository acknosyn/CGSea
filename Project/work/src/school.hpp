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

	float sphereRadius = 10.0;

	void update(bool); // run every frame

	void initialisePositions();

	void renderSchool();
	void renderSphere();
};