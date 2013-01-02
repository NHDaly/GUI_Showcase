
#ifndef __SOCKET_SERVER_H__
#define __SOCKET_SERVER_H__

#include <string>
#include <cstring>
#include <sys/types.h>

// TCP Socket Server
class Socket_Server
{
public:
    // NULL port will assign port number automatically.
    Socket_Server(uint16_t port, const int backlog_ = 10);
    ~Socket_Server();
    
    void listen();
    Socket_Server *accept();    // Return new socket created from accept()
    void send(std::string buffer, int flags = 0);
	std::string recv_all(int len, int flags = 0);
	std::string recv(int len, int flags = 0);
    
    template <typename T>
    void recv_into(T* dest, int flags = 0) {
        std::string msg = recv_all(sizeof(dest), flags);
        memcpy(dest, msg.c_str(), msg.size());
    }
    
    unsigned int get_addr() { return address; }
    unsigned int get_port() { return port_num; }
    
private:
    Socket_Server(int socket_fd, const int backlog_) 
    : sockfd(socket_fd), backlog(backlog_) { }
    
    int sockfd; 
    int backlog;
    
    unsigned int address;
    unsigned int port_num;
    
    // Turn these off
    Socket_Server(const Socket_Server& rhs);
    Socket_Server& operator=(const Socket_Server& rhs);
};

class Socket_Client
{
public:
    // NULL port will assign port number automatically.
    Socket_Client(uint16_t port, const int backlog_ = 10);
    ~Socket_Client();
    
    void connect();
    void send(std::string buffer, int flags = 0);
	std::string recv_all(int len, int flags = 0);
	std::string recv(int len, int flags = 0);
    
    unsigned int get_addr() { return address; }
    unsigned int get_port() { return port_num; }
    
private:
    Socket_Client(int socket_fd, const int backlog_) 
    : sockfd(socket_fd), backlog(backlog_) { }
    
    int sockfd; 
    int backlog;
    
    unsigned int address;
    unsigned int port_num;
    
    // Turn these off
    Socket_Client(const Socket_Client& rhs);
    Socket_Client& operator=(const Socket_Client& rhs);
};

#endif /* __SOCKET_SERVER_H__ */

