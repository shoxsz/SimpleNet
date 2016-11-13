cmake_minimum_required(VERSION 3.7)
project(SimpleNetAPI)
set(CMAKE_BUILD_TYPE Release)

set(SIMPLE_NET_DIRECTORY ${CMAKE_SOURCE_DIR}/src/)

#set the lib directory
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)

#not compiling utilities yet
add_library(SimpleNet STATIC
                ${SIMPLE_NET_DIRECTORY}simplenet.hpp
                ${SIMPLE_NET_DIRECTORY}socket_base.hpp
                ${SIMPLE_NET_DIRECTORY}socket_base.cpp
                ${SIMPLE_NET_DIRECTORY}network_message.hpp
                ${SIMPLE_NET_DIRECTORY}socket_stream.hpp
                ${SIMPLE_NET_DIRECTORY}tcp_acceptor.hpp
                ${SIMPLE_NET_DIRECTORY}tcp_acceptor.cpp
                ${SIMPLE_NET_DIRECTORY}tcp_stream.hpp
                ${SIMPLE_NET_DIRECTORY}tcp_stream.cpp
                ${SIMPLE_NET_DIRECTORY}udp_stream.hpp
                ${SIMPLE_NET_DIRECTORY}udp_stream.cpp)

#link ws2_32.lib if we are on windows
if(WIN32)
target_link_libraries(SimpleNet ws2_32)
endif(WIN32)

#set the lib directory
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#build examples
add_subdirectory(examples)