cmake_minimum_required(VERSION 3.7)

project(set_mac_address)

add_executable(set_mac_address set-mac-address.c)
target_include_directories(set_mac_address PRIVATE /usr/include/libnl3)
target_link_libraries(set_mac_address -lnl-route-3 -lnl-3)
