//---------------------------------------------------------------------------
//
// Jun Burden 2015
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

Terrain::Terrain() {
	//g_geometry = new Geometry("work/assets/dragon.obj");
}

Terrain::Terrain(string filename) {
	cout << filename << endl;
	g_geometry = new Geometry(filename);

}

void Terrain::renderTerrain() {
	glBegin(GL_TRIANGLES);
	  glColor3f(0.5, 0.5, 0.5);
  	glVertex3f(-10.0, 0, 10.0);
    glVertex3f(0, 0, -10.0);
    glVertex3f(10.0, 0, 10.0);
  glEnd();
	//g_geometry->renderGeometry();
}
