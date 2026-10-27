#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	printf("before fork, pid=%d\n",getpid());

	pid_t pid = fork();

	if(pid==0){
		printf("child: pid=%d,my parent=%d\n", getpid(), getpid());
		}else{
			printf("parent: pid=%d, child=%d\n", getpid(),pid);
			wait(NULL);
			printf("child done\n");
}
return 0;
}
