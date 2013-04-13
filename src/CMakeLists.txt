CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (laretz)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -std=c++0x")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

FIND_PACKAGE (Boost REQUIRED system)

SET (SRCS
	main.cpp
	server.cpp
	clientconnection.cpp
	)

ADD_EXECUTABLE (laretz WIN32
	${SRCS}
	)
TARGET_LINK_LIBRARIES (laretz ${Boost_SYSTEM_LIBRARY})
