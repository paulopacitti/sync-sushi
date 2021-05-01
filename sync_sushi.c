#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include "customer.h"

#define BAR_SIZE 5 // number of seats in the bar
#define CUSTOMERS_SIZE 20 // number of customers;

pthread_mutex_t mutex;
sem_t block;
int customers_eating = 0;
int customers_waiting = 0;
int must_wait = 0;

Customer customers[CUSTOMERS_SIZE]; // collection of customers;
pthread_t customers_threads[CUSTOMERS_SIZE]; // collection of customers;

void* dinner(void* arg) {}

int main()
{
  for(int i=0; i < CUSTOMERS_SIZE; i++) 
  {
	  customers[i].id = i;
	  customers[i].status = waiting;
    printf("Customer { id: %d, status: %d } \n", customers[i].id, customers[i].status);
	}

  /* inicia o mutex */
	pthread_mutex_init(&mutex, 0);

  	/* inicia o semaphore block */
	sem_init(&block, 0, 0);

	/* cria as Threads */
	for(i=0;i<NO_OF_CUSTOMERS;i++) {
		pthread_create(&customers_threads[i], 0, dinner, &customers[i]);
	}
  return 0;
}