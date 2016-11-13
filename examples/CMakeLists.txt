include_directories(${CMAKE_SOURCE_DIR}/src/)
add_executable(TcpSimpleClient simple_client.cpp)
add_executable(TcpSimpleServer simple_server.cpp)

target_link_libraries(TcpSimpleClient SimpleNet)
target_link_libraries(TcpSimpleServer SimpleNet)