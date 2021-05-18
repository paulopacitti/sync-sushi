#ifndef CUSTOMER_H
#define CUSTOMER_H
typedef enum
{
  waiting,
  eating,
  done
} State;

typedef struct
{
  int id;
  int hungry;
  int plate_index;
  State status;
} Customer;
#endif