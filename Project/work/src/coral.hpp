//---------------------------------------------------------------------------
//
// Jun Burden 2015
// Based on COMP308 Assignment 2 skeleton 2015
// by Taehyun Rhee, Joshua Scott, Ben Allen
//
//----------------------------------------------------------------------------

#pragma once

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "comp308.hpp"

// Type to represent a branch
struct branch {
	float length = 0;             // Length of the branch
	float yRot = 0;
	float xRot = 0;
	std::vector<int> children; // Pointers to branch children in m_branches
	// For the root
	comp308::vec3 translation;    // Translation (Only for the Root)
};

class Coral {

private:
	std::vector<branch> m_branches;
	
	int createBranch(float, float, float, float, float, float, int);

	void drawAxis(branch *, GLUquadric*);
	void renderBranch(branch *, GLUquadric*);

public:
	Coral(float, float, float, float, int);
	void renderCoral();
};
