cmake_minimum_required(VERSION 3.10)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)


add_library(Win32 STATIC ${SOURCE_FILES})
target_include_directories(Win32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
	Win32 PUBLIC
	fmt::fmt
	termcolor::termcolor
)