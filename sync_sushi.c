#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "customer.h"
#include "draw.h"

#define BAR_SIZE 5 // number of seats in the bar
#define CUSTOMERS_SIZE 20 // number of customers;

pthread_mutex_t mutex; // blocks the usage of updating customers_eating and customers_waiting
sem_t seats; // semaphore that tracks the amount of seats

int customers_eating = 0;
int customers_waiting = 0;
int must_wait = 0;

Customer customers[CUSTOMERS_SIZE]; // collection of customers;
pthread_t customers_threads[CUSTOMERS_SIZE]; // collection of customers;

int plates[BAR_SIZE];
int plate_index = 0;

void* dinner(void* arg)
{ 
	Customer* customer = (Customer*) arg;
	while(1)
	{
		pthread_mutex_lock(&mutex); // locks mutex
		if(must_wait) // if bar is full, customer has to wait
		{ 
			customers_waiting += 1;
			customer->status = waiting;
			
			pthread_mutex_unlock(&mutex); // releases mutex allowing counters update
			sem_wait(&seats); // blocks thread until a seat becomes available

			customers_waiting -= 1;
		} 

		customers_eating += 1;
		customer->status = eating;
    customer->plate_index = plate_index;
    plates[plate_index] = 6;
    plate_index = (plate_index+1)%BAR_SIZE;
		must_wait = (customers_eating == BAR_SIZE);
		if(customers_waiting && !must_wait) 
			sem_post(&seats); // a seat becomes available
		else
			pthread_mutex_unlock(&mutex); // releases mutex
    
    int time_to_eat = rand() % 3 + 1;
    while (plates[customer->plate_index] > 0)
    {
      plates[customer->plate_index] -= 1;
      sleep(time_to_eat);
    }
    //plates[customer->plate_index] = 6;
    
		pthread_mutex_lock(&mutex);
    customer->status = done;
		customers_eating -= 1;
		// customer->status = waiting;
		customer->hungry = 0;

		if(customers_eating == 0)
			must_wait = 0;
		if(customers_waiting && !must_wait) 
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
    customers[i].hungry = 6;
    customers[i].status = waiting;
    customers[i].plate_index = 0;
    printf("{ id: %d, status: %d, hungry: %d } \n", customers[i].id, customers[i].status, customers[i].hungry);
  }

  /* inicia o mutex */
	pthread_mutex_init(&mutex, 0);

  /* inicia o semaphore seats */
	sem_init(&seats, 0, 0);

	/* cria as Threads */
	for(int i=0;i< CUSTOMERS_SIZE; i++) {
		pthread_create(&customers_threads[i], 0, dinner, &customers[i]);
	}
  int frame_index = 0;
	while(1)
	{	
    usleep(100000);
		//sleep(1);
    system("@cls||clear");
    draw((int) frame_index%2==0, customers, customers_eating, plates);
    frame_index+=1;
    printf("=================================\n");
		printf("must_wait: %d\n", must_wait);
		printf("customers eating: %d\n", customers_eating);
		printf("customers waiting: %d\n", customers_waiting);

		for(int i=0;i< CUSTOMERS_SIZE; i++)
		{
			printf("{ id: %d, status: %d, hungry: %d } \n", customers[i].id, customers[i].status, customers[i].hungry);
		}
		
	}
  return 0;
}