#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include "customer.h"

#define BAR_SIZE 5 // number of seats in the bar
#define CUSTOMERS_SIZE 20 // number of customers;

sem_t mutex;
sem_t block;
int customers_eating = 0;
int customers_waiting = 0;
int must_wait = 0;

Customer customers[CUSTOMERS_SIZE]; // collection of customers;

int main()
{
	for(int i=0; i < CUSTOMERS_SIZE; i++) 
  {
	  customers[i].id = i;
	  customers[i].status = waiting;
    printf("Customer { id: %d, status: %d } \n", customers[i].id, customers[i].status);
	}
  return 0;
}