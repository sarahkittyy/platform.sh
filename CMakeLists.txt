cmake_minimum_required(VERSION 3.13)
project(platform.sh)

# c++17 lib
set(CMAKE_CXX_STANDARD 17)

# get sfml
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window audio network system)

if(NOT CMAKE_BUILD_TYPE EQUAL "DEBUG")
	set(SFML_LIB sfml-graphics sfml-window sfml-audio sfml-network sfml-system)
else()
	set(SFML_LIB sfml-graphics-d sfml-window-d sfml-audio-d sfml-network-d sfml-system-d)
endif()

# compile ImGui
add_subdirectory("lib/imgui/imgui")

# get sources
file(GLOB_RECURSE sources "src/*.cpp")

# create target
add_executable(platform.sh ${sources})
target_compile_options(platform.sh PUBLIC -Wall)
target_link_directories(platform.sh PUBLIC ${NFD_PATH})
target_include_directories(platform.sh PUBLIC 
	"include/" 
	"lib/nlohmann" 
	"lib/imgui")
target_link_libraries(platform.sh ${SFML_LIB} stdc++fs imgui)