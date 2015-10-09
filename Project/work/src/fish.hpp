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

class Fish {
private:
	comp308::vec3 position;
	comp308::vec3 velocity;
public:
	Fish();
	Fish(comp308::vec3);

	void renderFish();
};