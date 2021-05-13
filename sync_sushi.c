#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "customer.h"

#define BAR_SIZE 5 // number of seats in the bar
#define CUSTOMERS_SIZE 20 // number of customers;
#define RESTROOM_SIZE 2 //  number of toilet in the bar;


pthread_mutex_t mutex; // blocks the usage of updating customers_eating and customers_waiting_eat
sem_t seats; // semaphore that tracks the amount of seats
sem_t toilet; // semaphore that tracks the amount of toilet


int customers_eating = 0;
int customers_waiting_eat = 0;
int must_wait_eat = 0;
int customers_in_restroom = 0;
int customers_waiting_toilet =0;
int must_wait_toilet = 0;

Customer customers[CUSTOMERS_SIZE]; // collection of customers;
pthread_t customers_threads[CUSTOMERS_SIZE]; // collection of customers;

void* dinner(void* arg)
{ 
	Customer* customer = (Customer*) arg;
	while(1)
	{
		// seat wait verification
		pthread_mutex_lock(&mutex); // locks mutex
		if(must_wait_eat) // if bar is full, customer has to wait
		{ 
			customers_waiting_eat += 1;
			customer->status = waiting;
			
			pthread_mutex_unlock(&mutex); // releases mutex allowing counters update
			sem_wait(&seats); // blocks thread until a seat becomes available

			customers_waiting_eat -= 1;
		} 

		// customer start to eat
		customers_eating += 1;
		customer->status = eating;
		must_wait_eat = (customers_eating == BAR_SIZE);
		if(customers_waiting_eat && !must_wait_eat) 
			sem_post(&seats); // a seat becomes available
		else
			pthread_mutex_unlock(&mutex); // releases mutex

		if(customer->peemergency == 1)
		{
			// toilet wait verification
			pthread_mutex_lock(&mutex);
			if(must_wait_toilet) 
			{ 
				customers_waiting_toilet += 1;
				customer->status = waiting;
				pthread_mutex_unlock(&mutex); // releases mutex 
				sem_wait(&toilet); // blocks thread until a toilet becomes available

				customers_waiting_toilet -= 1;
			} 

			// customer start to toilet
			customers_in_restroom += 1;
			customer->usedRestroom = 1;
			customer->status = peeing;
			must_wait_toilet = (customers_in_restroom == RESTROOM_SIZE);
			if(customers_waiting_toilet && !must_wait_toilet) 
				sem_post(&toilet); // a toilet becomes available
			else
				pthread_mutex_unlock(&mutex); // releases mutex

			// customer finish to toilet
			pthread_mutex_lock(&mutex);
			customers_in_restroom -= 1;
			// customer->peemergency = 0;
			if (customers_in_restroom < RESTROOM_SIZE)
				must_wait_toilet = 0;
			if(customers_waiting_toilet && !must_wait_toilet) 
				sem_post(&toilet); // a toilet becomes available
			else
				pthread_mutex_unlock(&mutex); // releases mutex
		}

		// customer finish to eat
		pthread_mutex_lock(&mutex);
		customers_eating -= 1;
		customer->starved = 0;
		if(customers_eating == 0 && customers_in_restroom == 0 )
			must_wait_eat = 0;
		if(customers_waiting_eat && !must_wait_eat) 
			sem_post(&seats); // a customer can seats at the bar
		else
			pthread_mutex_unlock(&mutex); // releases mutex		
	}
}

int main()
{
  for(int i=0; i < CUSTOMERS_SIZE; i++) 
  {
		customers[i].id = i;
		customers[i].starved = 1;
		customers[i].peemergency = rand() % 2; // create a 50% chance to have a peemergency
		customers[i].usedRestroom = 0;
		customers[i].status = waiting;
		printf("{ id: %d, status: %d, usedRestroom = %d, starved: %d } \n", customers[i].id, customers[i].status, customers[i].usedRestroom, customers[i].starved);
	}

  	/* inicia o mutex */
	pthread_mutex_init(&mutex, 0);

  	/* inicia o semaphore */
	sem_init(&seats, 0, 0);
	sem_init(&toilet, 0, 0);

	/* cria as Threads */
	for(int i=0;i< CUSTOMERS_SIZE; i++) {
		pthread_create(&customers_threads[i], 0, dinner, &customers[i]);
	}

	while(1)
	{	
		sleep(1);
		printf("\n");
		printf("=============== Bar Table =================\n");
		printf("customers eating: %d\n", customers_eating);
		printf("customers waiting to eat: %d\n", customers_waiting_eat);
		printf("=============== Bar Restroom =================\n");
		printf("customers in toilet: %d\n", customers_in_restroom);
		printf("customers waiting to toilet: %d\n", customers_waiting_toilet);
		printf("=============== Legenda: =================\n");
		printf("status = 0: esperando\n");
		printf("status = 1: comendo\n");
		printf("status = 2: no banheiro\n");

		for(int i=0;i< CUSTOMERS_SIZE; i++)
		{
			printf("{ id: %d, status: %d, starved: %d } \n", customers[i].id, customers[i].status, customers[i].starved);
			customers[i].peemergency = rand() % 2; // create a 50% chance to have a peemergency
			customers[i].usedRestroom = 0;	// reset the usedRestroom
		}
		
	}
  return 0;
}