#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <string>
#include <string_view>
#include <sys/socket.h>
#include <unistd.h>

/*
It is a good practice to use int instead of bool for setsockopt options.
There is a sutble risk here. Since we are making a system call, the OS
might expect a specific size for the option value. If we pass a bool, it 
might be 1 byte or 4 bytes depending on the compiler and architecture. 
This could lead to unexpected behavior.
*/
constexpr int LOOPBACK = 1; // Whether to receive our own multicast packets
constexpr int TTL = 1; // Time-to-live for multicast packets (how many router hops are allowed)
constexpr int MULTICAST_PORT = 9000;

/*
It is safe to use any address in the range 239.0.0.0/8 as these are 
administratively scoped addresses. Addresses in this range are not 
forwarded by routers to other networks (similar to private IP addresses).
*/
constexpr std::string_view MULTICAST_GROUP = "239.1.1.1";

int main () {
    int sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock_fd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    sockaddr_in multicast_addr {};

    multicast_addr.sin_family = AF_INET;
    multicast_addr.sin_port = htons(MULTICAST_PORT);

    if (inet_pton(AF_INET, MULTICAST_GROUP.data(), &multicast_addr.sin_addr) <= 0) {
        std::cerr << "Invalid multicast address" << std::endl;
        close(sock_fd);
        return -1;
    }

    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_TTL, &TTL, sizeof(TTL)) < 0) {
        std::cerr << "Set TTL failed" << std::endl;
        close(sock_fd);
        return -1;
    }

    if (setsockopt(sock_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &LOOPBACK, sizeof(LOOPBACK)) < 0) {
        std::cerr << "Set loopback failed" << std::endl;
        close(sock_fd);
        return -1;
    }

    std::cout << "Multicast Server started\n";
    std::cout << "Broadcasting to " << MULTICAST_GROUP << ":" << MULTICAST_PORT << "\n";
    std::cout << "Type messages to broadcast (or 'quit' to exit):\n";

    std::string message;

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, message)) {
            break;
        }

        if (message == "quit") {
            break;
        }

        // Skip empty messages (user just pressed enter)
        if (message.empty()) {
            continue;
        }

        ssize_t bytes_sent = sendto(sock_fd, message.c_str(), message.size(), 0,
            reinterpret_cast<sockaddr*>(&multicast_addr), sizeof(multicast_addr));

        if (bytes_sent < 0) {
            std::cerr << "Error sending message" << std::endl;
            continue;
        }

        std::cout << "Broadcasted: '" << message << "' (" << bytes_sent << " bytes)\n";
    }

    close(sock_fd);
    std::cout << "Server shut down\n";

    return 0;
}
