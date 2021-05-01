typedef enum
{
  waiting,
  eating
} State;

typedef struct
{
  int id;
  State status;
} Customer;