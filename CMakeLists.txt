
cmake_minimum_required(VERSION 3.5)
project(SipTest VERSION 0.99 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.75 COMPONENTS system filesystem program_options REQUIRED)

if(NOT CMAKE_BUILD_TYPE)
 set(CMAKE_BUILD_TYPE "Release")
endif()

set(SOURCE
	${CMAKE_SOURCE_DIR}/source/UdpServer.cpp
	${CMAKE_SOURCE_DIR}/source/UdpServer.h
	${CMAKE_SOURCE_DIR}/source/UdpClient.cpp
	${CMAKE_SOURCE_DIR}/source/UdpClient.h
	${CMAKE_SOURCE_DIR}/source/Networking.cpp
	${CMAKE_SOURCE_DIR}/source/Networking.h
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
