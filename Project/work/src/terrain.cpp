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
	mcPoints = new vec4[(nX+1)*(nY+1)*(nZ+1)];
	vec3 stepSize((MAXX-MINX)/nX, (MAXY-MINY)/nY, (MAXZ-MINZ)/nZ);
	for(int i=0; i < nX+1; i++) {
		for(int j=0; j < nY+1; j++) {
			for(int k=0; k < nZ+1; k++) {
				vec4 vert(MINX+i*stepSize.x, MINY+j*stepSize.y, MINZ+k*stepSize.z, 0);
				vert.w = calculateDensity(vert);
				mcPoints[i*(nY+1)*(nZ+1) + j*(nZ+1) + k] = vert;
			}
		}
	}
	// Convert strut to geometry types
	vector<vec3> geoPoints;
	vector<triangle> geoTriangles;
	//runs Marching Cubes
	Triangles = MarchingCubesLinear(nX, nY, nZ, minValue, mcPoints, numOfTriangles);
	for(int i = 0; i<numOfTriangles; ++i) {
		geoPoints.push_back(Triangles[i].p[0]);
		geoPoints.push_back(Triangles[i].p[1]);
		geoPoints.push_back(Triangles[i].p[2]);
		triangle t; 
		t.v[0].p = geoPoints.size()-3;
		t.v[1].p = geoPoints.size()-2;
		t.v[2].p = geoPoints.size()-1;
		geoTriangles.push_back(t);
	}
	g_geometry = new Geometry(geoPoints, geoTriangles);
}

Terrain::Terrain(string filename) {
	cout << filename << endl;
	g_geometry = new Geometry(filename);
}

float Terrain::calculateDensity(vec4 coords) {
	Perlin p;
	float density = -coords.y;
	density += p.noise(coords.x, coords.y, coords.z) - 10.0003;
	density += p.noise(coords.x*0.403, coords.y*0.403, coords.z*0.403)*2.5;  
	density += p.noise(coords.x*0.196, coords.y*0.196, coords.z*0.196)*5.0;  
	density += p.noise(coords.x*0.101, coords.y*0.101, coords.z*0.101)*10.0;
	//density += p.noise(coords.x*0.05, coords.y*0.05, coords.z*0.05)*5.0;
	return density;


	// Ball
	//return coords.x*coords.x + coords.y*coords.y + coords.z*coords.z -50 - 0.0003;
	// Random terrain
	//return 2*sin(coords.x/4) + 0.1*sin(coords.x) + 0.1*sin(coords.x/3) + 0.1*sin(coords.x/3.5 + 15) + 0.2*sin(coords.z) + sin(coords.z/5)  -cos(coords.x/3.4 + coords.z/4.3) - coords.y - 5.003;
	// Slope
	//return coords.x*0.2-coords.y - 0.0003;
}

void Terrain::renderTerrain() {
	g_geometry->renderGeometry();}
