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
	float radius = 0;             // Radius of the branch
	float yRot = 0;
	float xRot = 0;
	std::vector<int> children; // Pointers to branch children in m_branches
};

class Coral {

private:
	std::vector<branch> m_branches;
	comp308::vec3 m_translation;    // Translation
	// Colour
	float m_r = 255.0f;
	float m_g = 0.0f;
	float m_b = 0.0f;
	
	int createBranch1(float, float, float, float, int);
	int createBranch2(float, float, float, float, int);
	int createBranch3(float, float, float, float, int);
	int createBranch4(float, float, float, float, int);

	void drawAxis(branch *, GLUquadric*);
	void renderBranch(branch *, GLUquadric*);

public:
	Coral(float, float, float, float, float, int, int);
	void changeColour(float, float, float);
	void renderCoral();
};
