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