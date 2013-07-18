cmake_minimum_required (VERSION 2.8)
project (laretz)

add_subdirectory (lib)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/lib)
set (FULLBUILD TRUE)
set (LARETZ_LIBRARIES "laretz_ops")
add_subdirectory (server)
