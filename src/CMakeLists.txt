CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (laretz)

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

FIND_PACKAGE (Boost REQUIRED filesystem serialization system thread)

SET (LIBOPS_SRCS
	item.cpp
	operation.cpp
	dbresult.cpp
	opsummer.cpp)

SET (SRCS
	main.cpp
	server.cpp
	clientconnection.cpp
	packetparser.cpp
	db.cpp
	dbmanager.cpp
	packetgenerator.cpp
	dboperator.cpp
	)

ADD_LIBRARY (laretz_ops SHARED ${LIBOPS_SRCS})

ADD_EXECUTABLE (laretz WIN32
	${SRCS}
	)
TARGET_LINK_LIBRARIES (laretz
	laretz_ops
	mongoclient
	crypto
	ssl
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SERIALIZATION_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_THREAD_LIBRARY}
	)
