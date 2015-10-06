//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <fstream>  // file streams
#include <sstream>  // string streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "terrain.hpp"

using namespace std;
using namespace comp308;


Terrain::Terrain(string filename) {
	cout<<filename<<endl;
}

void Terrain::renderTerrain() {
	glBegin(GL_TRIANGLES);
    glColor3f(0.5, 0.5, 0.5);
    glVertex3f(-1.0, 0, 1.0);
    glVertex3f(0, 0, -1.0);
    glVertex3f(1.0, 0, 1.0);
  glEnd();
}
