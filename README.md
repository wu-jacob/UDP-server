# UDP-server
A simple UDP server. I made this mainly to learn C++ socket programming.

## C++ Networking Library (or rather a lack thereof)

C++ does not include built-in networking capabilities in its standard library. As such, there is no standard way to do socket programming.

### POSIX's networking related APIs

POSIX stands for Portable Operating System Interface (for uniX) and is a famiily of standards to maintain compatibility between operating systems. This makes it eaiser to write software for these different Unix operating systems. Though not originally part of it, the Berkeley Sockets API was later incorporated into the POSIX standards. The POSIX standards are primarily written as C function specifications, meaning C and C++ code can directly call POSIX functions. This allows us to use the Berkeley Sockets API in our C++ code to make system calls to the kernel to send and receive data. 

### ASIO

ASIO (ASynchronous Input Output) is a cross platform C++ library that provides higher level abstractions for networking. Some of its main selling points are that it works well with modern C++ (co-routines, stronger type safety), can be asynchronous (as the name suggests), cross platform, and inspired the C++ networking TS. Most of the time you would probably want to choose ASIO over POSIX, except in performance critical scenarios where you can't afford the abstraction overhead (though at that point one might consider writing their own custom network stack or kernel bypass).

## Sockets

### High Level

At a high level, a socket is an endpoint for network communication. Think of it like a telephone where you can call/connect to another telephone. There are different types of sockets for different purposes (for example TCP/UDP).

### Low Level

Technically, sockets are just an abstraction provided by the OS. A socket API like Berkeley Sockets API provides an interface for us to make system calls to the kernel. The socket itself is just a data structure in kernel memory. When we create a socket, the kernel returns a file descriptor that points to an entry in that process's file descriptor table which points to a special inode that always points to memory (instead of disk space). This memory contains the data sturcture(s) that house the buffers for network communication.

## Multicast

Multicast allows for a single sender to send to multiple receivers at once. The sender sends packets to a multicast address (an address in a special range). Receivers will then have to join the multicast group associated with this address to receive the messages from the sender.

Routers and switches are responsible for filtering, forwarding, and duplicating packets as necessary. When a socket joins a multicast group, it makes a system call to the OS to tell it to listen for traffic on the associated multicast IP address. This means that the NIC and the OS will drop packets that don't match the multicast MAC filter, or that are not part of the any joined multicast group with at least one interested socket respectively. In addition to this, the OS sends an IGMP (Internet Group Management Protocol) "join" message to the local router. This message essentially tells the router to forward packets sent to the multicast address. The router then sends a PIM (Protocol Independent Multicast) join message upstream towards the central point in the multicast distribution tree. The process is similar for leaving a group. The actual logic on how this data is passed between routers and switches is a bit complex (especially when considering how to optimize this process) but just think of it as a mechanism for letting other routers and switches know about the new update (might talk more in detail about this in the future). Keep in mind that while it seems like multicast is plug and play, beyond the local subnet, it often requires manual configurations across routers and switches. 

In case it wasn't obvious already, multicast uses UDP and not TCP because TCP is connection oriented (one-to-one) while multicast is connectionless (one-to-many). Packet loss is expected and acceptable (or people have other mechanisms to handle this) and there are no acknowledgements sent back.
