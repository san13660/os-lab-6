// Christopher Sandoval 13660
// Lab 6 - Ejercicio 1

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/syscall.h>

#define THREADS 5
#define ITERATIONS 3

sem_t mutex;
int available_resources = 10;

int decrease_count(int count)
{
	if (available_resources < count)
	{
		return -1;
	}
	else
	{
		available_resources -= count;
		return 0;
	}
}


int increase_count(int count)
{
	available_resources += count;
	return 0;
}

int monitor(int action, int count)
{
	sem_wait(&mutex);
	int result = -1;
	if(action == 0)
	{
		result = decrease_count(count);
		printf("\n[MONITOR] - Recursos reducidos (%d)", available_resources);
	}
	else if(action == 1)
	{
		result = increase_count(count);	
		printf("\n[MONITOR] - Recursos incrementados (%d)", available_resources);
	}
	else
	{
		result = -1;
	}


	sem_post(&mutex);
	return result;
}

void* thread(void* arg)
{
	int thread_id = syscall(SYS_gettid);
	printf("\nIniciando thread: [%d]", thread_id);
	sleep(1);
	
	int i;
	for(i=0;i<ITERATIONS;i++)
	{
		printf("\n[%d] - Iniciando iteracion: %d", thread_id, i);
		while(1)
		{
			printf("\n[%d] - Solicitando (3) recursos", thread_id);
			if(monitor(0, 3) == 0)
			{
				printf("\n[%d] - Recursos tomados", thread_id);
				sleep((rand()%4)+1);
				printf("\n[%d] - Buenos dias! Recurso usado", thread_id);
				monitor(1, 3);
				printf("\n[%d] - Recursos devueltos", thread_id);
				break;
			}
			else
			{
				printf("\n[%d] - Recursos insuficientes", thread_id);
				sleep(1);
			}
		}
	}
	printf("\n[%d] - Thread finalizado!", thread_id);
}

int main()
{
	printf("\n[Main] - Iniciando programa");
	printf("\n[Main] - Recursos disponibles (%d)", available_resources);

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
