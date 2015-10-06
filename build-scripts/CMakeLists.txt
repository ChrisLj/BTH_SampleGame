cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(BTH_SampleGame)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/../SampleGame/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../SampleGame/*.h"
)

set(LIBRARIES
	SDL2
	SDL2main
	glew32
	glu32
	opengl32)

set(RELPATH "${CMAKE_CURRENT_SOURCE_DIR}/../")
get_filename_component(RELPATH ${RELPATH} ABSOLUTE)
foreach(f ${SOURCES})
    file(RELATIVE_PATH SRCGR ${RELPATH} ${f})
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../externals/include")

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../externals/lib/x64")

add_executable(BTH_SampleGame ${SOURCES})

target_link_libraries(BTH_SampleGame ${LIBRARIES})

add_custom_command(TARGET BTH_SampleGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/../externals/lib/x64"
		$<TARGET_FILE_DIR:BTH_SampleGame>)