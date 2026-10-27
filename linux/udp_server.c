#include <stdio.h>
#include <string.h>
#include <sys/socket.h>    // socket/recvfrom/sendto
#include <netinet/in.h>    // sockaddr_in
#include <arpa/inet.h>
#include <unistd.h>        // close

int main() {
    // 1. 创建UDP socket：SOCK_DGRAM（TCP是SOCK_STREAM）
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. 绑定端口8888
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    printf("udp server waiting on port 8888...\n");

    // 3. 直接收数据：没有listen/accept，等数据到
    char buf[100] = {0};
    struct sockaddr_in client;      // 记录谁发的
    socklen_t len = sizeof(client);
    int n = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr *)&client, &len);
    printf("received %d bytes: %s\n", n, buf);

    // 4. 回数据：发回给client
    sendto(fd, "hello udp client", 16, 0, (struct sockaddr *)&client, len);

    close(fd);
    return 0;
}
