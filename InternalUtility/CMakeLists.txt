cmake_minimum_required(VERSION 3.10)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)

add_library(InternalUtility STATIC ${SOURCE_FILES})

target_include_directories(InternalUtility PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
	InternalUtility PUBLIC
	WindowsWrapper
	Maths
	fmt::fmt
)