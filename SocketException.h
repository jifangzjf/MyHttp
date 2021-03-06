#ifndef SOCKETEXCEPTION_H_INCLUDED
#define SOCKETEXCEPTION_H_INCLUDED

#include <string>
#include <string.h>
#include <errno.h>

class SocketException
{
public:
    typedef std::string string;
    SocketException(const string &_msg = string())
        : msg(_msg) {}
    string what() const
    {
        if (errno == 0)
            return msg;
        return msg + ": " + strerror(errno);
    }

private:
    string msg;
};

#endif // SOCKETEXCEPTION_H_INCLUDED
