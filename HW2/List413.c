#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// Count all works, that was executed by process
// ----------------------------------------------------------
int works_count = 0;

// Exclude mutex & semaphore 
// ----------------------------------------------------------
pthread_mutex_t job_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t job_queue_count;

// It is the function for our work
// ----------------------------------------------------------
void task(const int* value){
	printf(" [work] | now working on %d.\n", *value);
}

// job structure is used to create queue of works
// ----------------------------------------------------------
struct job {
	struct job* next;
	void (*func)(const int*);
};
struct job* job_queue;

// process_job used for execute tasks of every job
// ----------------------------------------------------------
void process_job(struct job* jb, const int* value){
	printf(" [prep] | preparing to work.\n");
	jb->func(value);
}

// Just initialize semaphore & job_queue
// ----------------------------------------------------------
void initialize_job_queue() {
	job_queue = NULL;
	sem_init(&job_queue_count, 0, 0);
}

// Thread function : here we call execution of our tasks
// if job queue is not empty. else we will wait for new job.
// ----------------------------------------------------------
void* thread_function(void* arg) {
	while (1) {
		struct job* next_job;
		sem_wait(&job_queue_count);
		pthread_mutex_lock(&job_queue_mutex);
		next_job = job_queue;
		job_queue = job_queue->next;
		pthread_mutex_unlock(&job_queue_mutex);
		process_job(next_job, &works_count);
		works_count++;
		free(next_job);
	}
	return NULL;
}

// function add job work to job_queue
// ----------------------------------------------------------
void enqueue_job() {
	struct job* new_job;

	new_job = (struct job*)malloc(sizeof(struct job));

	pthread_mutex_lock(&job_queue_mutex);
	new_job->next = job_queue;
	job_queue = new_job;
	job_queue->func = &task;

	sem_post(&job_queue_count);
	pthread_mutex_unlock(&job_queue_mutex);
}

// in main we start execution thread and them add new tasks
// randomly
// ATTENTION : As execution thread has the same speed of
// execution as main, we can`t see many jobs in queue.
// IF you want to see more that 1 work in job_queue, remove
// usleep() function and redirect program output to file to 
// have a possibility to analyze output.
// ----------------------------------------------------------
void main(int argc, char* argv[]){
	int tasks = 0;	
	int rnd_value = 0;
	pthread_t thread;
	pthread_create(&thread, NULL, &thread_function, NULL);
	
	while(1){
		srand(time(NULL) + rnd_value);
		rnd_value = rand()%2;
		if( rnd_value == 0){
			enqueue_job();
			sem_getvalue(&job_queue_count, &tasks);
			printf(" [main] | new task was added. Tasks now : %d.\n", tasks);	
		}
		usleep(1000);
	}		

	pthread_join(thread, NULL);
	return;
}


