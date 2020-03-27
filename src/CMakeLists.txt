project( LoggerTest )

file( GLOB_RECURSE ${PROJECT_NAME}_SOURCES "*.cpp" )

add_executable( ${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} )

target_link_libraries( ${PROJECT_NAME} PUBLIC Logger_Lib )
