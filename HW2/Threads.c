#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct params{
	int result;
	int value;
};

void* sum(void* param){
	struct params* pars = (struct params*)param;	
	pars->result = ((1 + pars->value)*pars->value)/2;
	return NULL;
}

void* pthread_attr_mod(void* attr){
	int ret;
	int stack_sz;
	void* stack_adr;	

	ret = pthread_attr_init(attr);
	if(ret != 0){	
		printf(" [error] | cannot init stack to attributes (error code : %d).\n", ret);
		return NULL;
	}

	printf(" Enter size of stack in BYTES ('0' to use default [1MB]) : ");
	if(scanf("%d", &stack_sz) < 0){
		printf(" [error] | something goes wrong when scanned value.\n");
	}
	if(stack_sz == 0)
		stack_sz = 1048576;	
		
	ret = pthread_attr_setstack(attr, malloc(stack_sz), stack_sz);
	if(ret != 0){	
		printf(" [error] | cannot set stack to attributes (error code : %d).\n", ret);
		return NULL;
	}
}

int main(){
	struct params par = {0, 10};	
	pthread_attr_t attr;	
	pthread_attr_mod(&attr);
	pthread_t thread;
	
	printf(" Enter number : ");
	scanf("%d", &(par.value));
	pthread_create(&thread, &attr, &sum, &par);
	pthread_join(thread, NULL);	
	
	printf(" Result is %d.\n", par.result);

	return 0;
}
