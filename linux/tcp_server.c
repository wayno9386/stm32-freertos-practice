#include <stdio.h>
#include <string.h>
#include <sys/socket.h>    // socket/accept/send/recv
#include <netinet/in.h>    // sockaddr_in
#include <arpa/inet.h>
#include <unistd.h>        // close

int main() {
    // 1. 创建socket：AF_INET=IPv4，SOCK_STREAM=TCP
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. 绑定端口8888
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);          // htons：端口转网络字节序
    addr.sin_addr.s_addr = INADDR_ANY;    // 接受任意IP连接
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    // 3. 监听，最多5个等待队列
    listen(fd, 5);
    printf("server waiting on port 8888...\n");

    // 4. 接受连接（阻塞在这，等客户端来）
    int cfd = accept(fd, NULL, NULL);
    printf("client connected!\n");

    // 5. 收数据
    char buf[100] = {0};
    int n = recv(cfd, buf, sizeof(buf), 0);
    printf("received %d bytes: %s\n", n, buf);

    // 6. 回数据
    send(cfd, "hello client", 12, 0);

    // 7. 关闭
    close(cfd);
    close(fd);
    return 0;
}
