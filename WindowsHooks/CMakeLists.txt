cmake_minimum_required(VERSION 3.11)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)

add_library(WindowsHooks STATIC ${SOURCE_FILES})
target_include_directories(WindowsHooks PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
