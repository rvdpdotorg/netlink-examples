/*
BSD 3-Clause License

Copyright (c) 2020, Ronald van der Pol
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <link.h>
#include "netlink/route/link.h"
#include "netlink/route/addr.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr,
                "usage: set_ipv6_address <interface> <ipv6-addres/prefix>\n");
        exit(EXIT_FAILURE);
    }
    char *name = argv[1];
    char *ipv6prefix = argv[2];
    struct nl_sock *nl_sk = nl_socket_alloc();
    if (nl_sk == NULL) {
        fprintf(stderr, "nl_socket_alloc failed\n");
        exit(EXIT_FAILURE);
    }
    int result = nl_connect(nl_sk, NETLINK_ROUTE);
    if (result < 0) {
        nl_perror(result, "nl_connect");
        exit(EXIT_FAILURE);
    }
    struct nl_cache *link_cache;
    result = rtnl_link_alloc_cache(nl_sk, AF_UNSPEC, &link_cache);
    if (result < 0) {
        nl_perror(result, "rtnl_link_alloc_cache");
        exit(EXIT_FAILURE);
    }
    struct rtnl_link *link = rtnl_link_get_by_name(link_cache, name);
    if (link == NULL) {
        fprintf(stderr, "rtnl_link_get_by_name: %s not found.\n", name);
        exit(EXIT_FAILURE);
    }
    int ifindex = rtnl_link_get_ifindex(link);
    if (ifindex == 0) {
        fprintf(stderr, "rtnl_link_get_ifindex failed\n");
        exit(EXIT_FAILURE);
    }
    printf("%s had index %d\n", name, ifindex);

    struct nl_addr *ipv6_addr;
    result = nl_addr_parse(ipv6prefix, AF_INET6, &ipv6_addr);
    if (result < 0) {
        nl_perror(result, "nl_addr_parse");
        exit(EXIT_FAILURE);
    }
    struct rtnl_addr *addr = rtnl_addr_alloc();
    if (addr == NULL) {
        fprintf(stderr, "rtnl_addr_alloc failed\n");
        exit(EXIT_FAILURE);
    }
    rtnl_addr_set_ifindex(addr, ifindex);
    result = rtnl_addr_set_local(addr, ipv6_addr);
    if (result < 0) {
        nl_perror(result, "rtnl_addr_set_local");
        exit(EXIT_FAILURE);
    }
    result = rtnl_addr_add(nl_sk, addr, NLM_F_CREATE | NLM_F_EXCL);
    if (result < 0) {
        nl_perror(result, "rtnl_addr_add");
        exit(EXIT_FAILURE);
    }
    rtnl_addr_put(addr);
    nl_socket_free(nl_sk);
}
