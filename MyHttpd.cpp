#include "commen.h"

void *task(void *args) throw(SocketException)
{
    TCPClient client = *(TCPClient *)args;
    char page[128] = {0};
    try
    {
        // 解析HTTP报头, 获取需要返回的页面
        getPage(client, page);
        // 返回页面
        writePage(client, page);
    }
    catch (const SocketException &e)
    {
        syslog(LOG_ERR, "catch: %s\n", e.what().c_str());
    }
    delete (TCPClient *)args;
    return NULL;
}

int main()
{
    signal(SIGPIPE, SIG_IGN);
    signal(SIGCHLD, sigHanderForSigChild);

    int port;       // Server 绑定的端口号
    int threads;    // Server 最大能够启动的线程数
    char home[128] = {0};   // Server 的家目录(保存着主要的html文件与图片文件)
    // 读取配置文件, 确定 {端口, 最大线程数, 根目录}
    if (configuration(port, threads, home) == false)
    {
        if (errno != 0)
            err_exit("configuration error");
        else
            exit(EXIT_FAILURE);
    }
    //设置守护进程
    makeDaemon(home, 0);

    try
    {
        TCPServer server(port);
        ThreadPool pool(threads);
        TCPClient client;
        while (true)
        {
            server.accept(client);
            // 添加任务到线程池的任务队列
            pool.addTask(task, new TCPClient(client));
        }
    }
    catch (const SocketException &e)
    {
        syslog(LOG_ERR, "catch: %s\n", e.what().c_str());
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
