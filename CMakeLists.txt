
cmake_minimum_required(VERSION 3.5)
project(SipTest VERSION 0.99 LANGUAGES C CXX)
include(ExternalProject)

set(tinyxml2_prefix ${CMAKE_CURRENT_BINARY_DIR}/tinyxml2)
set(tinyxml2_INSTALL_DIR ${tinyxml2_prefix}/install)

set(patch_command ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/patches/tinyxml2.cpp.patch <SOURCE_DIR> && git checkout <SOURCE_DIR>/tinyxml2.cpp && git apply <SOURCE_DIR>/tinyxml2.cpp.patch)

ExternalProject_Add(tinyxml2
  PREFIX ${tinyxml2_PREFIX}
  GIT_REPOSITORY https://github.com/leethomason/tinyxml2
  GIT_TAG tags/7.1.0
  UPDATE_DISCONNECTED 1
  PATCH_COMMAND ${patch_command}
  CMAKE_ARGS
    -DBUILD_SHARED_LIBS=OFF
    -DBUILD_TESTS=OFF
    -DHTTP_ONLY=ON
    -DCMAKE_INSTALL_PREFIX=${tinyxml2_INSTALL_DIR}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DCMAKE_GENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}
)
set(tinyxml2_INSTALL_DIR ${tinyxml2_INSTALL_DIR} PARENT_SCOPE)
set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.75 COMPONENTS system filesystem program_options REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
 set(CMAKE_BUILD_TYPE "Release")
endif()

set(SOURCE
	${CMAKE_SOURCE_DIR}/source/utilities.cpp
	${CMAKE_SOURCE_DIR}/source/UdpServer.cpp
	${CMAKE_SOURCE_DIR}/source/UdpServer.h
	${CMAKE_SOURCE_DIR}/source/UdpClient.cpp
	${CMAKE_SOURCE_DIR}/source/UdpClient.h
	${CMAKE_SOURCE_DIR}/source/Networking.cpp
	${CMAKE_SOURCE_DIR}/source/Networking.h
	${CMAKE_SOURCE_DIR}/source/SipMethods.h
	${CMAKE_SOURCE_DIR}/source/SipMethods.cpp
	${CMAKE_SOURCE_DIR}/source/SipScenario.h
	${CMAKE_SOURCE_DIR}/source/SipScenario.cpp
	${CMAKE_SOURCE_DIR}/source/tinyxml2/tinyxml2.h
	${CMAKE_SOURCE_DIR}/source/tinyxml2/tinyxml2.cpp
	${CMAKE_SOURCE_DIR}/source/main.cpp)

# ADD EXECUTABLE
add_executable(SipTest ${SOURCE})

#if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    target_link_libraries(SipTest ${Boost_LIBRARIES})
#endif()

if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SipTest)
	set_target_properties( SipTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/Build" CXX_STANDARD 17)
	set_target_properties( SipTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/Build" CXX_STANDARD 17)
#	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
endif(MSVC)
