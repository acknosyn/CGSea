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

using namespace comp308;

class Fish {
private:
	vec3 position;
	vec3 velocity;

public:
	Fish();

	void renderFish();
};