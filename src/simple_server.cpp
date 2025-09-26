#include <iostream>
#include <netinet/in.h> // For sockaddr_in, IPPROTO_UDP, IDNADDR_ANY, htons, etc.
#include <string>
#include <sys/socket.h> // For socket functions
#include <unistd.h> // For close()

int main() {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    /*
    Initialize to zero as we don't set sin_zero explicitly
    sockaddr_in has a sin_zero field to pad the struct to 16 bytes (size of sockaddr)
    sockaddr is a generic address struct for all types of sockets, so we need to cast sockaddr_in to sockaddr
    */
    sockaddr_in server_addr {};

    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = INADDR_ANY; // Accept connections from any IP address
    server_addr.sin_port = htons(8080); // htons converts to network byte order (big-endian)

    /*
    Note that we use reinterpret_cast to cast sockaddr_in* to sockaddr*
    Even though this is unsafe, it is required since bind expects a sockaddr*
    Static cast would not work here as sockaddr_in and sockaddr do not share a parent-child relationship

    The bind function binds the socket to the specified IP address and port 
    (in this case any address on port 8080)
    */
    if (bind(sock_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        close(sock_fd);
        return -1;
    }

    std::cout << "Simple server is listening on port 8080" << std::endl;

    while (true) {
        char buffer[1024] {};
        sockaddr_in client_addr {};
        socklen_t client_len = sizeof(client_addr);

        ssize_t bytes_read = recvfrom(sock_fd, buffer, sizeof(buffer) - 1, 0, 
            reinterpret_cast<sockaddr*>(&client_addr), &client_len);

        if (bytes_read < 0) {
            std::cerr << "Error receiving data" << std::endl;
            continue;
        }

        buffer[bytes_read] = '\0'; // Null terminate
        std::cout << "Received: " << buffer << std::endl;

        std::string response = "Echo: " + std::string(buffer);
        sendto(sock_fd, response.c_str(), response.size(), 0,
            reinterpret_cast<sockaddr*>(&client_addr), client_len);
    }

    /*
    Close releases the file descriptor number back to the OS
    It unbinds the port and frees kernel memory associated with the socket
    */
    close(sock_fd);
    return 0;
};
