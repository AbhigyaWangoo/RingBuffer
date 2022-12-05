#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

const size_t ring_size = 10;
int cur = 0;
int buff[ring_size];

pthread_mutex_t m;
pthread_cond_t cv_cons;
pthread_cond_t cv_prod;


void* produce(void* arg) {
	// add a value to the ring buffer. If the buffer is full, wait.
	int* argi = (int*) arg;

	pthread_mutex_lock(&m);
	
	while (cur >= ring_size) {
		printf("Waiting as producer\n");
		pthread_cond_wait(&cv_prod, &m);
	}
	
	printf("producing when cur is %d, and the argument is %d\n", cur, *argi);
	buff[cur] = *argi;
	cur++;
	pthread_mutex_unlock(&m);
	
	pthread_cond_signal(&cv_cons);

	pthread_exit(NULL);
}

void* consume(void* arg) {
	// remove and read a value from the ring buffer. If the ring buffer is empty, wait.
	int res;

	pthread_mutex_lock(&m);
	
	while (cur <= 0) {
		printf("Waiting as consumer\n");
		pthread_cond_wait(&cv_cons, &m);
	}
	
	printf("consuming when cur is %d\n", cur);
	res = buff[cur];
	printf("%d\n", res);
	cur--;
	pthread_mutex_unlock(&m);
	
	pthread_cond_signal(&cv_prod);

	

	pthread_exit(NULL);
}

int main(int argc, char** argv) {
	pthread_t pthreads[40];
	int random;

	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&cv_cons, NULL);
	pthread_cond_init(&cv_prod, NULL);

	for (int i = 0; i < 20; i++) {
		random = rand();
		// printf("random number start: %d\n", random);
		if (pthread_create(pthreads + i, NULL, produce, (void*) &random) != 0) {
			perror("Couldn't produce");
		}
	}
	for (int i = 20; i < 40; i++) {
		if (pthread_create(pthreads + i, NULL, consume, NULL) != 0) {
			perror("Couldn't consume");
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (pthread_join(pthreads[i], NULL) != 0) {
			perror("Couldn't finish");
		}
	}

	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&cv_cons);
	pthread_cond_destroy(&cv_prod);

	return 0;
}
