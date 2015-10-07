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
	g_geometry = new Geometry("work/assets/dragon.obj");
}

Terrain::Terrain(string filename) {
	cout << filename << endl;
	g_geometry = new Geometry("work/assets/dragon.obj");

}

void Terrain::renderTerrain() {
	g_geometry->renderGeometry();
}
