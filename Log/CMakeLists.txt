cmake_minimum_required(VERSION 3.11)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)


add_library(Log STATIC ${SOURCE_FILES})
target_include_directories(Log PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(../dependencies/termcolor build-termcolor EXCLUDE_FROM_ALL)
add_subdirectory(../dependencies/fmt build-fmt EXCLUDE_FROM_ALL)

target_link_libraries(
	Log PUBLIC
	fmt::fmt
	termcolor::termcolor
)