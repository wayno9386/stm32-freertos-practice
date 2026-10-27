#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
	int fd = open("test.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd,"hello linux\n",12);
	close(fd);


	fd = open("test.txt", O_RDONLY);
	char buf[100] = {0};
	int n = read(fd,buf,sizeof(buf));
	printf("read %d bytes: %s", n, buf);
	close(fd);
	return 0;
}
