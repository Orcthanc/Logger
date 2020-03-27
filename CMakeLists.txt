cmake_minimum_required( VERSION 3.7 )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

project( Logger )

option( BUILD_TESTS "Build test programs" OFF )

add_library( ${PROJECT_NAME}_Lib INTERFACE )

target_include_directories( ${PROJECT_NAME}_Lib INTERFACE "include" )

if( BUILD_TESTS )
	add_subdirectory( src )
endif( BUILD_TESTS )
