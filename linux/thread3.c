/*
该程序演示信号量来实现同步
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

void *thread_function(void *arg);
sem_t bin_sem;

/*工作区域*/
#define WORK_SIZE 1024
char work_area[WORK_SIZE];

int main(){
	int res;
	pthread_t a_thread;
	void *thread_result;

	/*初始化信号量*/
	res = sem_init(&bin_sem, 0, 0);
	if(res != 0){
		perror("Semaphore initialization failed");
		exit(EXIT_FAILURE);
	}

	/*创建线程*/
	res = pthread_create(&a_thread, NULL, thread_function, NULL);
	if(res != 0){
		perror("Thread creation failed");
		exit(EXIT_FAILURE);
	}

	/*添加资源，设置信号量*/
	printf("Input some text. Enter 'end to finish'\n");
	while(strncmp("end", work_area, 3) != 0){
		fgets(work_area, WORK_SIZE, stdin);
		sem_post(&bin_sem);
	}

	printf("\nWaiting for thread to finish...\n");

	/*等待线程结束*/
	res = pthread_join(a_thread, &thread_result);
	if(res != 0){
		perror("Thread join failed");
		exit(EXIT_FAILURE);
	}

	printf("Thread joined\n");
	sem_destroy(&bin_sem);
	exit(EXIT_SUCCESS);
}

/*线程回调函数*/
void *thread_function(void *arg){
	/*等待资源，资源减1*/
	sem_wait(&bin_sem);
	while(strncmp("end", work_area, 3) != 0){
		printf("You input %d characters\n", strlen(work_area)-1);
		/*等待资源，资源减1*/
		sem_wait(&bin_sem);
	}
	pthread_exit(NULL);
}