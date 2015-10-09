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
	Fish schoolOfFish [10];
	int sphereRadius = 10;

public:
	School();

	void renderSchool();
	void renderSphere();

	void initialisePositions();
};