
cmake_minimum_required(VERSION 3.5)
project(SipTest VERSION 0.99 LANGUAGES C CXX)
include(ExternalProject)

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
	${CMAKE_SOURCE_DIR}/source/SipHelper.h
	${CMAKE_SOURCE_DIR}/source/SipHelper.cpp
	${CMAKE_SOURCE_DIR}/source/SipScenario.h
	${CMAKE_SOURCE_DIR}/source/SipScenario.cpp
	${CMAKE_SOURCE_DIR}/source/tinyxml2/tinyxml2.h
	${CMAKE_SOURCE_DIR}/source/tinyxml2/tinyxml2.cpp
	${CMAKE_SOURCE_DIR}/source/XMLParser.h
	${CMAKE_SOURCE_DIR}/source/XMLParser.cpp
	${CMAKE_SOURCE_DIR}/source/main.cpp
	${CMAKE_SOURCE_DIR}/source/scenarios
	)

# ADD EXECUTABLE
add_executable(SipTest ${SOURCE})
include_directories(${CMAKE_SOURCE_DIR}/source/scenarios)
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
