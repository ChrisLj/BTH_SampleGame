cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(BTH_SampleGame)

file(GLOB_RECURSE SOURCES
	"${CMAKE_CURRENT_SOURCE_DIR}/../source/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/../source/*.h"
)

set(LIBRARIES
	SDL2
	SDL2main
	glew32
	glu32
	zdll
	zzip)

set(RELPATH "${CMAKE_CURRENT_SOURCE_DIR}/../")
get_filename_component(RELPATH ${RELPATH} ABSOLUTE)
foreach(f ${SOURCES})
    file(RELATIVE_PATH SRCGR ${RELPATH} ${f})
    string(REGEX REPLACE "(.*)(/[^/]*)$" "\\1" SRCGR ${SRCGR})
    string(REPLACE / \\ SRCGR ${SRCGR})
    source_group("${SRCGR}" FILES ${f})
endforeach()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../bin/debug/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/../bin/debug/")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/release/")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../bin/release/")

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/../externals/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/../BTH_Memory_Allocator/src/memoryallocator/interface"
	"${CMAKE_CURRENT_SOURCE_DIR}/../BTH_Resource_Manager/src/resourcemanager/interface"
)

link_directories("${CMAKE_CURRENT_SOURCE_DIR}/../externals/lib/x64")

add_executable(BTH_SampleGame ${SOURCES})

if(NOT TARGET MemoryAllocator)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../BTH_Memory_Allocator/src/memoryallocator" MemoryAllocator)
endif()
if(NOT TARGET ResourceManager)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../BTH_Resource_Manager/src/resourcemanager" ResourceManager)
endif()

target_link_libraries(BTH_SampleGame
					  ${LIBRARIES}
					  MemoryAllocator
					  ResourceManager)

add_custom_command(TARGET BTH_SampleGame POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/../externals/lib/x64"
		$<TARGET_FILE_DIR:BTH_SampleGame>)