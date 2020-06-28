# Netlink Examples
I was looking for information about how to set an IPv6 address to an interface
programmatically. I had set a MAC address via the ioctl(2) system call. But this
interface is considered old and has been replaced with the
[netlink(7)](https://www.man7.org/linux/man-pages/man7/netlink.7.html)
framework. I had trouble finding good up to date documentation. I came across
the [Netlink Library Suite](https://github.com/thom311), which is a slightly
higher level abstraction and easier to use API. I decided to write down a couple
of simple programs that use this framework.

* set-interface-up (bring an interface from DOWN to UP state)
* set-ipv6-address (assign an IPv6 address including prefix to an interface)
* set-mac-address (set the Ethernet MAC address of an interface)

## How To Compile And Run The Examples
Create a build directory. Go to it and run "cmake .." and make:

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

The executables are placed in subdirectories of the build directory.

Create an interface with e.g.:

    $ sudo ip link add name veth100 type veth peer name veth101
    $ ip addr show dev veth100
    7: veth100@veth101: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN group default qlen 1000
        link/ether 5a:d7:36:93:d7:c9 brd ff:ff:ff:ff:ff:ff

Configure the MAC address:

    $ sudo ./set_mac_address veth100 00:00:00:01:02:03
    veth100 had index 7
    $ ip addr show dev veth100
    7: veth100@veth101: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN group default qlen 1000
        link/ether 00:00:00:01:02:03 brd ff:ff:ff:ff:ff:ff

Add an IPv6 address:

    $ sudo ./set_ipv6_address veth100 2001:db8::42/64
    veth100 had index 7
    $ ip addr show dev veth100
    7: veth100@veth101: <BROADCAST,MULTICAST,M-DOWN> mtu 1500 qdisc noop state DOWN group default qlen 1000
        link/ether 00:00:00:01:02:03 brd ff:ff:ff:ff:ff:ff
        inet6 2001:db8::42/64 scope global tentative 
            valid_lft forever preferred_lft forever
            
Bring the interface up (including its peer veth101):

    $ sudo ./set_interface_up veth100
    veth100 had index 7
    $ ip addr show dev veth100
    7: veth100@veth101: <NO-CARRIER,BROADCAST,MULTICAST,UP,M-DOWN> mtu 1500 qdisc noqueue state LOWERLAYERDOWN group default qlen 1000
        link/ether 00:00:00:01:02:03 brd ff:ff:ff:ff:ff:ff
        inet6 2001:db8::42/64 scope global tentative 
            valid_lft forever preferred_lft forever
    $ sudo ./set_interface_up veth101
    veth101 had index 6
    $ ip addr show dev veth100
    7: veth100@veth101: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default qlen 1000
        link/ether 00:00:00:01:02:03 brd ff:ff:ff:ff:ff:ff
        inet6 2001:db8::42/64 scope global 
            valid_lft forever preferred_lft forever
        inet6 fe80::200:ff:fe01:203/64 scope link 
            valid_lft forever preferred_lft forever
