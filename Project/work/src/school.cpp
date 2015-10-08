//---------------------------------------------------------------------------
//
// Francesco Badraun 2015
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
#include "school.hpp"

using namespace std;
using namespace comp308;

School::School() {
	Fish first = Fish();
	schoolOfFish[1] = {first};
}

void School::renderSchool() {
	schoolOfFish[0].renderFish();
}
