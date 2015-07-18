#include "commen.h"

bool configuration(int &port, int &threads, char *path)
{
    FILE *fp = fopen("./config.ini", "r");
    if (fp == NULL)
        return false;

    char buf[BUFSIZ] = {0};
    while (fgets(buf, sizeof(buf), fp) != NULL)
    {
        //注释, 不用解析
        if (buf[0] == '#')
            continue;

        if (buf[strlen(buf)-1] != '\n')
        {
            cerr << "error in config.ini" << endl;
            return false;
        }
        else
            buf[strlen(buf)-1] = '\0';

        char *p = NULL;
        if (strstr(buf, "port") == buf)
        {
            if ((p = strchr(buf, ':')) == NULL)
            {
                cerr << "config.ini expect ':'" << endl;
                return false;
            }
            port = atoi(p+2);
            if (port <= 0)
            {
                cerr << "error port" << endl;
                return false;
            }
        }
        else if (strstr(buf, "root-path") == buf)
        {
            if ((p = strchr(buf, ':')) == NULL)
            {
                cerr << "config.init expect ':'" << endl;
                return false;
            }
            strcpy(path, p+2);
        }
        else if (strstr(buf, "threads") == buf)
        {
            if ((p = strchr(buf, ':')) == NULL)
            {
                cerr << "config.init expect ':'" << endl;
                return false;
            }
            threads = atoi(p+2);
            if (threads <= 0)
            {
                cerr << "error threads" << endl;
                return false;
            }
        }
        else
        {
            cerr << "error in config.ini\n" << endl;
            return false;
        }
        memset(buf, 0, sizeof(buf));
    }
    return true;
}
void makeDaemon(char *home, bool noclose)
{
    umask(0);

    pid_t pid = fork();
    if (pid == -1)
        err_exit("fork error");
    else if (pid != 0)   //parent
        exit(0);

    setsid();

    if (home != NULL)
        chdir(home);
    if (noclose == 0)
    {
        int i;
        for (i=0; i < 3; ++i)
            close(i);
        open("/dev/null", O_RDWR);  //相当于把0号文件描述符指向/dev/null
        dup(0); //把0号文件描述符 赋值给空闲的文件描述符 1
        dup(0); //把0号文件描述符 赋值给空闲的文件描述符 2
    }
}
void getPage(TCPClient &client, char *page)
throw(SocketException)
{
    char buf[BUFSIZ] = {0};
    try
    {
        client.read(buf, sizeof(buf));
    }
    catch (...)
    {
        //向上层重新抛出错误
        throw;
    }
    if ((buf[4] == '/') && (buf[5] == ' '))
        strcpy(page, "index.html");
    else
    {
        strtok(&buf[4], " ");
        strcpy(page, &buf[5]);
    }
}

//写回错误页面
static void writeErrorPage(TCPClient &client)
throw(SocketException)
{
    try
    {
        client.write("HTTP/1.1 404 File Not Exist\r\n");
        client.write("Content-Type: text/html\r\n\r\n");
        int fd = open("./errpage.html", O_RDONLY);
        if (fd == -1)
            err_exit("open errpage error");
        char buf[BUFSIZ] = {0};
        while (read(fd, buf, sizeof(buf)-1) > 0)
        {
            client.write(buf);
            memset(buf, 0, sizeof(buf));
        }
    }
    catch (...)
    {
        throw;
    }
}

//写回正常页面
static void writeRegularPage(TCPClient &client, int fd, char *page)
throw(SocketException)
{
    //写回HTTP头部类型
    int n = strlen(page);
    try
    {
        client.write("HTTP/1.1 200 OK\r\nContent-Type: ");
        if (strcasecmp(&page[n-3], "jpg") == 0 ||
                strcasecmp(&page[n-4],"jpeg") == 0)
            client.write("image/jpeg");
        else if (strcasecmp(&page[n-3], "gif") == 0)
            client.write("image/gif");
        else if (strcasecmp(&page[n-3], "png") == 0)
            client.write("image/png");
        else
            client.write("text/html");
        client.write("\r\n\r\n");

        char buf[BUFSIZ] = {0};
        int readBytes = 0;
        while ((readBytes = read(fd, buf, sizeof(buf))) > 0)
        {
            if (write(client.getfd(), buf, readBytes) == -1)
                throw SocketException("write socket error");
            memset(buf, 0, sizeof(buf));
        }
    }
    catch (...)
    {
        throw;
    }

}

void writePage(TCPClient &client, char *page)
throw(SocketException)
{
    int fd = open(page, O_RDONLY);
    try
    {
        if (fd == -1)   //文件不存在, 写回错误页面
            writeErrorPage(client);
        else            //写回正常页面
            writeRegularPage(client, fd, page);
    }
    catch (const SocketException &e)
    {
        //重新抛出
        throw;
    }
}
