cmake_minimum_required(VERSION 3.10)

file(
	GLOB SOURCE_FILES
	src/*.cpp
)

add_library(Obfuscation STATIC ${SOURCE_FILES})

target_include_directories(Obfuscation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(
		Obfuscation PUBLIC
)