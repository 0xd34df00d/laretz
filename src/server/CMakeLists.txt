cmake_minimum_required (VERSION 2.8)
project (laretz_server)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pthread")

find_package (Boost REQUIRED filesystem serialization system thread)

if (NOT FULLBUILD)
	set (CMAKE_MODULE_PATH "/usr/local/share/apps/cmake/modules;/usr/share/apps/cmake/modules")
	find_package (LibLaretz REQUIRED)
endif ()

include_directories (${LARETZ_INCLUDE_DIRS})

set (SRCS
	main.cpp
	server.cpp
	clientconnection.cpp
	itemmongo.cpp
	db.cpp
	dbmanager.cpp
	dboperator.cpp
	)

add_executable (laretz WIN32
	${SRCS}
	)
target_link_libraries (laretz
	${LARETZ_LIBRARIES}
	mongoclient
	crypto
	ssl
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SERIALIZATION_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_THREAD_LIBRARY}
	)

install (TARGETS laretz DESTINATION "bin")
