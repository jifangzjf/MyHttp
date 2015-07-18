#include "Socket.h"

/** Socket基类 **/
TCPSocket::TCPSocket(): m_sockfd(-1) {}
TCPSocket::~TCPSocket()
{
    if (isValid())
        ::close(m_sockfd);
}

bool TCPSocket::create()
{
    if (isValid())
        return false;

    if ((m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return false;
    return true;
}

bool TCPSocket::bind(unsigned short int port, const char *ip) const
{
    if (!isValid())
        return false;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (ip == NULL)
        addr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
        addr.sin_addr.s_addr = inet_addr(ip);
    if ( ::bind(m_sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1 )
        return false;
    return true;
}
bool TCPSocket::listen(int backlog) const
{
    if (!isValid())
        return false;

    if ( ::listen(m_sockfd, backlog) == -1)
        return false;
    return true;
}
bool TCPSocket::accept(TCPSocket &clientSocket) const
{
    if (!isValid())
        return false;

    clientSocket.m_sockfd =
        ::accept(this->m_sockfd, NULL, NULL);
    if (clientSocket.m_sockfd == -1)
        return false;
    return true;
}

bool TCPSocket::connect(unsigned short int port, const char *ip) const
{
    if (!isValid())
        return false;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);
    if ( ::connect(m_sockfd, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
        return false;
    return true;
}

bool TCPSocket::setNonBlocking(bool flag) const
{
    if (!isValid())
        return false;
    int opt = fcntl(m_sockfd, F_GETFL, 0);
    if (opt == -1)
        return false;
    if (flag)
        opt |= O_NONBLOCK;
    else
        opt &= ~O_NONBLOCK;
    if (fcntl(m_sockfd, F_SETFL, opt) == -1)
        return false;
    return true;
}
bool TCPSocket::reuseaddr() const
{
    if (!isValid())
        return false;

    int on = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1)
        return false;
    return true;
}
bool TCPSocket::close()
{
    if (!isValid())
        return false;
    ::close(m_sockfd);
    m_sockfd = -1;
    return true;
}

/** Server TCP Socket**/
TCPServer::TCPServer(unsigned short int port, const char *ip, int backlog)
throw(SocketException)
{
    if (create() == false)
        throw SocketException("tcp server create error");
    if (reuseaddr() == false)
        throw SocketException("tcp server reuseaddr error");
    if (bind(port, ip) == false)
        throw SocketException("tcp server bind error");
    if (listen(backlog) == false)
        throw SocketException("tcp server listen error");
}
TCPServer::~TCPServer() {}
void TCPServer::accept(TCPClient &client) const
throw(SocketException)
{
    //显式调用基类TCPSocket的accept
    if (TCPSocket::accept(client) == -1)
        throw SocketException("tcp server accept error");
}

/** client TCP Socket **/
TCPClient::TCPClient(unsigned short int port, const char *ip) throw(SocketException)
{
    if (create() == false)
        throw SocketException("tcp client create error");
    if (connect(port, ip) == false)
        throw SocketException("tcp client connect error");
}
TCPClient::TCPClient() {}
TCPClient::~TCPClient() {}
size_t TCPClient::write(const char *msg) throw(SocketException)
{
    if ( ::write(m_sockfd, msg, strlen(msg)) == -1 )
        throw SocketException("tcp client write error");
    return strlen(msg);
}
void TCPClient::write(const void *buf, size_t count) throw(SocketException)
{
    if ( ::write(m_sockfd, buf, count) == -1 )
        throw SocketException("tcp client write error");
}
size_t TCPClient::read(void *buf, size_t count) throw(SocketException)
{
    ssize_t readBytes = ::read(m_sockfd, buf, count);
    if (readBytes == -1)
        throw SocketException("tcp client read error");
    return (size_t)readBytes;
}
