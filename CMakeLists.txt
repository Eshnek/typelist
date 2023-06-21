cmake_minimum_required(VERSION 3.25)

set(ProjectName typelist)

set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SourcesGlob CONFIGURE_DEPENDS tests/*.cxx)
add_executable(typelist ${SourcesGlob})
