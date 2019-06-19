#include <string>
#include <unistd.h>
#include <asm/ioctls.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "UdpClient.h"


UdpClient::UdpClient() {
}

UdpClient::~UdpClient() {
    cleanupSocket();
}

void UdpClient::initializeSocket() {

    //create a new UPD socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_socket < 0) {
        int error = errno;
        cleanupSocket();
        throw std::string{"Create socket failed: " + error};
    }

    sockaddr_in sock_address{};
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = m_ipAddress;//INADDR_ANY;
    sock_address.sin_port = m_port;

    int bindResult = bind(m_socket, (const sockaddr *) &sock_address, sizeof(sock_address));
    if (bindResult < 0) {
        int error = errno;
        cleanupSocket();
        throw std::string{"Bind socket failed (ipv4): " + error};
    }

    if (fcntl(m_socket, F_SETFL, O_NONBLOCK) != 0) {
        int error = errno;
        cleanupSocket();
        throw std::string{"Failed to make socket non-blocking: " + error};
    }
}

void UdpClient::cleanupSocket() {
    if (m_socket > 0) {
        shutdown(m_socket, SHUT_RDWR);
        close(m_socket);
    }
}

void UdpClient::Connect(const char *ipAddress, const u_short port) {
    m_ipAddress = inet_addr(ipAddress);
    m_port = htons(port);

    initializeSocket();
}

int UdpClient::Send(const sockaddr_in &to, const uint8_t *buffer, const size_t bufferSize) const {
    int sent_bytes = sendto(m_socket, (const char *) buffer, bufferSize, 0, (sockaddr *) &to, sizeof(sockaddr_in));

    bool result = sent_bytes == bufferSize;

    if (!result) {
        throw std::string{"sendto() failed with error code: " + errno};
    }

    return sent_bytes;
}

int UdpClient::Receive(sockaddr_in &from, uint8_t *buffer, const size_t bufferSize) const  {
    socklen_t fromLength = sizeof(from);

    int bytes_received = recvfrom(m_socket, (char *) buffer, bufferSize, 0, (sockaddr *) &from, &fromLength);

    if (bytes_received <= 0) {
        int error = errno;
        if (error == EWOULDBLOCK) {
            //socket is non-blocking, so ignore this error
        } else if (error == EMSGSIZE) {
            throw std::string{"recvfrom() failed with error code: " + errno}
                    + std::string{", bytes_received: " + bytes_received}
                    + std::string(", bufferSize: " + bufferSize);
        } else {
            throw std::string{"recvfrom() failed with error code : " + errno};
        }
        return 0;
    }

    return bytes_received;
}