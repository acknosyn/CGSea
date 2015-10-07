//---------------------------------------------------------------------------
//
// Jun Burden 2015
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "comp308.hpp"
#include "geometry.hpp"

class Terrain {
private:
	Geometry *g_geometry = nullptr;

public:
	Terrain();
	Terrain(std::string);

	void renderTerrain();
};