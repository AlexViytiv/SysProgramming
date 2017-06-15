#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int forkF();
int execF();
int systF();

int main(int argc, char *argv[]){
	printf(" [MainT] | The main program process ID is %d\n", (int)getpid());
	
	setsid();
	systF();	
	forkF(argv);
	execF(argv);
	daemon(1,1);	

}

int forkF(char *argv[]){
	int ret_code;
	pid_t fork_child_pid;
	
	fork_child_pid = fork();
	if (fork_child_pid != 0) {
		printf(" [ForkT] | This is the parent process, with ID %d\n", (int)getpid());
		printf(" [ForkT] | PID of process %d\n", (int)fork_child_pid);
		wait(&fork_child_pid);
	} else {
		printf(" [ForkT] | PID of process %d\n", (int)getpid());
		return 0;
	}
}

int execF(char *argv[]){
	pid_t exec_child_pid;
	int ret_value;		
	printf(" [ExecT] | PID of process %d\n", (int)getpid());	
	
	exec_child_pid = execl("ls", "ls", argv, NULL);	
	_exit(EXIT_FAILURE);

	return ret_value;
}

int systF(){
	int ret;
	ret = system("sleep 2");	
	printf(" [SystT] | PID of process %d\n", (int)getpid());	
	return ret;
}
