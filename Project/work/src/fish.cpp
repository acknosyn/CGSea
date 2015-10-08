//---------------------------------------------------------------------------
//
// Francesco Badraun 2015
//
//----------------------------------------------------------------------------

#include <cmath>
#include <iostream> // input/output streams
#include <string>
#include <stdexcept>
#include <vector>

#include "comp308.hpp"
#include "fish.hpp"

using namespace std;
using namespace comp308;

Fish::Fish() {
	// init
}

void Fish::renderFish() {

	glPushMatrix(); {
		glColor3f(0.1, 0.2, 0.5);
		glutSolidSphere(2, 10, 10);
	}
	glPopMatrix();
}
