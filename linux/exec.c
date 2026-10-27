#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
pid_t pid = fork();
if(pid==0){
printf("child will run ls\n");
execlp("ls","ls","-l", NULL);
printf("exec failed\n");
}else{
wait(NULL);
printf("parent: child finished\n");
}
return 0;
}
