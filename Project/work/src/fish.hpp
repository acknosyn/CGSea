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

struct fishProperties {

	comp308::vec3 position;
	comp308::vec3 velocity;

};

class Fish {
private:
	fishProperties prop;
public:
	Fish();

	void renderFish();
};