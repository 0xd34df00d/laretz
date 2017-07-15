cmake_minimum_required (VERSION 2.8)
project (laretz_libs)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

find_package (Boost REQUIRED serialization)

set (LIBOPS_SRCS
	item.cpp
	operation.cpp
	opsummer.cpp
	packetparser.cpp
	packetgenerator.cpp
	)

set (LIBOPS_HEADERS
	item.h
	operation.h
	opsummer.h
	packetparser.h
	packetgenerator.h
	laretzversion.h
	)

include_directories(${Boost_INCLUDE_DIRS})
add_library (laretz_ops SHARED ${LIBOPS_SRCS})
target_link_libraries (laretz_ops
	${Boost_SERIALIZATION_LIBRARY}
	)

if (NOT CMAKE_INSTALL_LIBDIR)
	set (CMAKE_INSTALL_LIBDIR lib)
endif ()

install (TARGETS laretz_ops DESTINATION ${CMAKE_INSTALL_LIBDIR})
install (FILES ${LIBOPS_HEADERS} DESTINATION "include/laretz")
install (FILES FindLibLaretz.cmake DESTINATION "share/cmake/Modules")
