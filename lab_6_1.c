// Christopher Sandoval 13660
// Lab 6 - Ejercicio 1

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

#define THREADS 5
#define ITERATIONS 5

sem_t mutex;

void* thread(void* arg)
{
	int thread_id = syscall(SYS_gettid);
	printf("\nIniciando thread: [%d]", thread_id);
	sleep(1);
	
	int i;
	for(i=0;i<ITERATIONS;i++)
	{
		printf("\n[%d] - Iniciando iteracion: %d", thread_id, i);
		//wait
		sem_wait(&mutex);
		printf("\n[%d] - Recurso tomado", thread_id);

		//critical section
		sleep((rand()%4)+1);
		printf("\n[%d] - Buenos dias! Recurso usado", thread_id);

		//signal
		sem_post(&mutex);
		printf("\n[%d] - Recurso devuelto", thread_id);
	}
	printf("\n[%d] - Thread finalizado!", thread_id);
}

int main()
{
	printf("\n[Main] - Iniciando programa");

	sem_init(&mutex, 0, 1);
	int i;
	pthread_t threads[THREADS];
	for(i=0;i<THREADS;i++)
	{
		pthread_create(&threads[i],NULL,thread,NULL);
	}
	printf("\n[Main] - Esperando threads...");
	for(i=0;i<THREADS;i++)
	{
		pthread_join(threads[i],NULL);
	}
	sem_destroy(&mutex);

	printf("\n[Main] - Programa finalizado\n");
	return 0;
}
