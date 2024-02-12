cmake_minimum_required(VERSION 3.10)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)

add_library(WindowsWrapper STATIC ${SOURCE_FILES})
target_include_directories(WindowsWrapper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
	WindowsWrapper PUBLIC
	InternalUtility
)