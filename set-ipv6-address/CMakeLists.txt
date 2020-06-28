cmake_minimum_required(VERSION 3.7)

project(set_ipv6_address)

add_executable(set_ipv6_address set-ipv6-address.c)
target_include_directories(set_ipv6_address PRIVATE /usr/include/libnl3)
target_link_libraries(set_ipv6_address -lnl-route-3 -lnl-3)
