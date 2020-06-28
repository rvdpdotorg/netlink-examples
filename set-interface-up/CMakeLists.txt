cmake_minimum_required(VERSION 3.7)

project(set_interface_up)

add_executable(set_interface_up set-interface-up.c)
target_include_directories(set_interface_up PRIVATE /usr/include/libnl3)
target_link_libraries(set_interface_up -lnl-route-3 -lnl-3)
