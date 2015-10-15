#
# COMP 308
# If you add new source files, you must list them here
#

# TODO list your header files (.hpp) here
SET(headers
	"comp308.hpp"
	"terrain.hpp"
	"geometry.hpp"
	"marchingCubes.hpp"
	"mcTable.hpp"
	"perlin.hpp"
	"coral.hpp"
	"fish.hpp"
	"school.hpp"
	"shaderLoader.hpp"
)


# TODO list your source files (.cpp) here
SET(sources
	"main.cpp"
	"terrain.cpp"
	"geometry.cpp"
	"marchingCubes.cpp"
	"perlin.cpp"
	"coral.cpp"
	"fish.cpp"
	"school.cpp"
)

# Add executable target and link libraries
# You do not need to touch this
add_executable(${COMP308_ASSIGNMENT} ${headers} ${sources})
target_link_libraries(${COMP308_ASSIGNMENT} PRIVATE GLUT::GLUT glew)
