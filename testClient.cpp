#include "commen.h"

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigHanderForSigChild);
    int connectNumber = 0;
    while (cin >> connectNumber)
    {
        for (int i = 0; i < connectNumber; ++i)
        {
            int sockfd = socket(AF_INET, SOCK_STREAM, 0);
            if (sockfd == -1)
                err_exit("socket error");
            struct sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_port = htons(8080);
            serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
            if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
                err_exit("connect error");
            //close(sockfd);
        }
    }
}
