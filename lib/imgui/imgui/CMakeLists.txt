# grab all imgui sources
file(GLOB_RECURSE sources "*.cpp")

add_library(imgui ${sources})
target_include_directories(imgui PUBLIC "${CMAKE_SOURCE_DIR}/lib/imgui/imgui")
target_link_libraries(imgui GL)