if(NOT (${CMAKE_BINARY_DIR} STREQUAL "${CMAKE_SOURCE_DIR}/build"))
	message(FATAL_ERROR "Must be in \"build\" directory")
endif()

#Init
cmake_minimum_required(VERSION 3.5.1)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
set(shmea_FOUND 1)

#Compiler Flags
set(CMAKE_BUILD_TYPE Debug) # -g
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#Project
project(shmea)
set(G_VERSION_MAJOR 0)
set(G_VERSION_MINOR 55)

set(DEFAULT_BUILD_TYPE "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

#Subdirectories
include_directories(${CMAKE_SOURCE_DIR})
include_directories("services")
add_subdirectory("Backend/Database")
add_subdirectory("Backend/Networking")

set(MAIN_SRC_FILES
	main.cpp
	main.h
)

add_library(${PROJECT_NAME} SHARED ${MAIN_SRC_FILES})

#Link libraries
target_link_libraries(${PROJECT_NAME}
	GNet DB) #custom libs

target_include_directories(shmea PRIVATE "include")
target_include_directories(shmea PRIVATE "Backend")

#target_include_directories(${PROJECT_NAME} PUBLIC
#    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#    $<INSTALL_INTERFACE:include>
#    PRIVATE Backend)

message(${CMAKE_INSTALL_LIBDIR})

install(TARGETS ${PROJECT_NAME} EXPORT shmeaConfig
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})

install(EXPORT shmeaConfig DESTINATION share/shmea/cmake)

export(TARGETS ${PROJECT_NAME} FILE shmeaConfig.cmake)

#make fmt
add_custom_target(fmt ALL
	COMMAND ./clang-format-all
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

#make local
add_custom_target(local
	COMMAND ${PROJECT_NAME} local
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
#make docs
add_custom_target(docs
	COMMAND google-chrome index.html
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/docs/html/
)

#make debug
add_custom_target(debug
	COMMAND gdb ./build/${PROJECT_NAME}
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

#make mem
	add_custom_target(mem
	COMMAND valgrind --leak-check=full --track-origins=yes ./build/${PROJECT_NAME}
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

#make profile
	add_custom_target(profile
	COMMAND valgrind --tool=callgrind ./build/${PROJECT_NAME}
	DEPENDS ${PROJECT_NAME}
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)