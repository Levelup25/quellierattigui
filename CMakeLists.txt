cmake_minimum_required (VERSION 2.8)
project (plt)

# Directories
include_directories ("${PROJECT_SOURCE_DIR}/src/client")
include_directories ("${PROJECT_SOURCE_DIR}/src/server")
include_directories ("${PROJECT_SOURCE_DIR}/src/shared")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

# Enable C++11
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

IF(CMAKE_COMPILER_IS_GNUCC)
	SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall -std=c11   -pthread -g -pg")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread -g -pg")
ENDIF(CMAKE_COMPILER_IS_GNUCC)


# Generate headers
set(DIA2CODE "${PROJECT_SOURCE_DIR}/bin/dia2code")
exec_program(${DIA2CODE} ARGS -ns state  -d ../src/shared -t cpp ../src/state.dia)
#exec_program(${DIA2CODE} ARGS -ns render -d ../src/client -t cpp ../src/render.dia)
#exec_program(${DIA2CODE} ARGS -ns engine -d ../src/shared -t cpp ../src/engine.dia)
#exec_program(${DIA2CODE} ARGS -ns ai     -d ../src/shared -t cpp ../src/ai.dia)
#exec_program(${DIA2CODE} ARGS -ns client -d ../src/client -t cpp ../src/module.dia)
#exec_program(${DIA2CODE} ARGS -ns server -d ../src/server -t cpp ../src/module.dia)

# Sources
file(GLOB_RECURSE client_sources src/client/*.cpp)
#file(GLOB_RECURSE server_sources src/server/*.cpp)
file(GLOB_RECURSE shared_sources src/shared/*.cpp)

#jsoncpp
#include_directories ("${PROJECT_SOURCE_DIR}/extern/jsoncpp-1.8.0")
#file(GLOB_RECURSE jsoncpp_sources extern/jsoncpp-1.8.0/*.cpp)


# Executables
add_executable(client ${client_sources} ${shared_sources})
#add_executable(server ${server_sources} ${shared_sources} ${jsoncpp_sources})

# SFML
if(WIN32)
set(SFML_ROOT "D:/Utils/SFML-2.3")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/cmake/Modules" ${CMAKE_MODULE_PATH})
else(WIN32)
set(CMAKE_MODULE_PATH "/usr/share/SFML/cmake/Modules/" ${CMAKE_MODULE_PATH})
endif(WIN32)
if($ENV{MACHINE_ENSEA})
set(SFML_ROOT "/usr/lsa")
set(CMAKE_MODULE_PATH "${SFML_ROOT}/share/SFML/cmake/Modules/" ${CMAKE_MODULE_PATH})
endif($ENV{MACHINE_ENSEA})
find_package(SFML 2 COMPONENTS graphics window system network)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(client sfml-graphics sfml-window sfml-system sfml-network)
endif()

# libmicrohttpd
find_library(
	MHD_LIBRARY
	NAMES microhttpd microhttpd-10 libmicrohttpd libmicrohttpd-dll
	DOC "microhttpd library"
)
#target_link_libraries(server ${MHD_LIBRARY})

