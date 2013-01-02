#include "NathanSocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

Socket_Server::Socket_Server(uint16_t port, const int backlog_)
: backlog(backlog_)
{
    // Creates a socket. PF_INET == IPv4; SOCK_STREAM == TCP
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("failed to create socket");
        throw -1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // this is an IPv4 address
    // sin_addr == 0; meaning is "listen on all local network interfaces"
    // sin_port == 0; meaning is "OS, please assign me a port"
    addr.sin_port = htons(port); // convert from host to network byte order

    socklen_t addrlen = sizeof(addr);

    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    // bind() assigns the address to the socket.
    int rc = bind(sockfd, (struct sockaddr *) &addr, addrlen);
    if(rc < 0) {
        perror("bind failed");
        throw -1;
    }

    // update addr with (possibly OS-assigned) port number
    rc = getsockname(sockfd, (struct sockaddr *) &addr, &addrlen);
    if(rc < 0) {
        perror( "getsockname failed");
        throw -1;
    }

    address = ntohl(addr.sin_addr.s_addr);
    port_num = ntohs(addr.sin_port);
}

Socket_Server::~Socket_Server()
{
    close(sockfd);
}

void Socket_Server::listen()
{
     if (::listen(sockfd, backlog) < 0) { // (:: because names overlap)
        perror("failed to listen");
        throw 1;
    }
}

Socket_Server *Socket_Server::accept()
{
    int new_fd = ::accept(sockfd, NULL, 0);
 
    if (new_fd < 0) {
        perror("failed to accept new client");
        throw 1;
    }
        
    return new Socket_Server(new_fd, backlog);
}

void Socket_Server::send(string buffer, int flags)
{
	int data_sent = ::send(sockfd, buffer.c_str(), buffer.size(), flags);
    if (data_sent < 0) {
        perror("failed to send data");
		throw 1;
	}
}

string Socket_Server::recv_all(int len, int flags)
{
	char *buffer = new char[len];

	int total_recv = 0;

	// Keep receiving until data length == len
	while(total_recv < len) {
		int data_recv = ::recv(sockfd, buffer + total_recv, len - total_recv, flags);
		if (data_recv < 0) {
			throw -1;
		}
		total_recv += data_recv;
	}

    string result(buffer, total_recv);
    
    delete [] buffer;

    return result;
}

string Socket_Server::recv(int len, int flags)
{
	char *buffer = new char[len];
	
	int data_recv = ::recv(sockfd, buffer, len, flags);
	if (data_recv < 0) {
		throw -1;
	}
    
    string result(buffer, data_recv);
    
    delete [] buffer;
    return result;
}





Socket_Client::Socket_Client(uint16_t port, const int backlog_)
: backlog(backlog_)
{
    // Creates a socket. PF_INET == IPv4; SOCK_STREAM == TCP
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("failed to create socket");
        throw -1;
    }
    
    port_num = port;
    address = 0;
}

Socket_Client::~Socket_Client()
{
    close(sockfd);
}

void Socket_Client::connect()
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; // this is an IPv4 address
    // sin_addr == 0; meaning is "listen on all local network interfaces"
    // sin_port == 0; meaning is "OS, please assign me a port"
    addr.sin_port = htons(port_num); // convert from host to network byte order
    
    socklen_t addrlen = sizeof(addr);
    
    int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    
    if (::connect(sockfd, (struct sockaddr *) &addr, addrlen) == -1) {
        close(sockfd);
        perror("failed to connect");
        throw 1;
    }
    
    // update addr with (possibly OS-assigned) port number
    int rc = getsockname(sockfd, (struct sockaddr *) &addr, &addrlen);
    if(rc < 0) {
        perror( "getsockname failed");
        throw -1;
    }
    
    address = ntohl(addr.sin_addr.s_addr);
    port_num = ntohs(addr.sin_port);

}


void Socket_Client::send(string buffer, int flags)
{
	int data_sent = ::send(sockfd, buffer.c_str(), buffer.size(), flags);
    if (data_sent < 0) {
        perror("failed to send data");
		throw 1;
	}
}

string Socket_Client::recv_all(int len, int flags)
{
	char *buffer = new char[len];
    
	int total_recv = 0;
    
	// Keep receiving until data length == len
	while(total_recv < len) {
		int data_recv = ::recv(sockfd, buffer + total_recv, len - total_recv, flags);
		if (data_recv < 0) {
            perror("failed to recv data");
			throw -1;
		}
		total_recv += data_recv;
	}
    
    string result(buffer, total_recv);
    
    delete [] buffer;
    
    return result;
}

string Socket_Client::recv(int len, int flags)
{
	char *buffer = new char[len];
	
	int data_recv = ::recv(sockfd, buffer, len, flags);
	if (data_recv < 0) {
        perror("failed to recv data");
		throw -1;
	}
    
    string result(buffer, data_recv);
    
    delete [] buffer;
    return result;
}

















