/**主要包含:
    1. MyHttpd 所包含的头文件
    2. Server端主要的业务实现代码
**/
#ifndef COMMENT_H_INCLUDED
#define COMMENT_H_INCLUDED

// 自定义 Header
#include "Socket.h"
#include "threadpool.h"

// C++ Header
#include <iostream>
#include <string>
using std::cerr;
using std::endl;
using std::cin;

// Linux Header
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <arpa/inet.h>

// ANSI C Header
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool configuration(int &port, int &threads, char *path);
void makeDaemon(char *home, bool noclose);
void getPage(TCPClient &client, char *page) throw(SocketException);
void writePage(TCPClient &client, char *page) throw(SocketException);

inline void sigHanderForSigChild(int signo)
{
    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;
}
inline void err_exit(std::string msg)
{
    syslog(LOG_ERR, "%s: %s\n", msg.c_str(), strerror(errno));
    exit(EXIT_FAILURE);
}

#endif // COMMENT_H_INCLUDED
