cmake_minimum_required(VERSION 3.20)

project(CppBallGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE BALLGAME_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

set(BALLGAME_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
set(BALLGAME_CORE_SOURCES ${BALLGAME_SOURCES})
list(FILTER BALLGAME_CORE_SOURCES EXCLUDE REGEX "/main\\.cpp$")

add_executable(BallGame ${BALLGAME_MAIN} ${BALLGAME_CORE_SOURCES})

add_executable(BallGameTest
	"${CMAKE_CURRENT_SOURCE_DIR}/tests/test_logic.cpp"
	${BALLGAME_CORE_SOURCES}
)

find_package(raylib QUIET)
if(NOT raylib_FOUND)
	include(FetchContent)
	set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
	set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
	set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
	FetchContent_Declare(
		raylib
		GIT_REPOSITORY https://github.com/raysan5/raylib.git
		GIT_TAG 5.0
	)
	FetchContent_MakeAvailable(raylib)
endif()

target_link_libraries(BallGame PRIVATE raylib)
target_link_libraries(BallGameTest PRIVATE raylib)

if(MSVC)
	target_compile_options(BallGame PRIVATE /W4 /utf-8)
	target_compile_options(BallGameTest PRIVATE /W4 /utf-8)
else()
	target_compile_options(BallGame PRIVATE -Wall -Wextra -Wpedantic)
	target_compile_options(BallGameTest PRIVATE -Wall -Wextra -Wpedantic)
endif()
