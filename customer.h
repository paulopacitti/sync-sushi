typedef enum
{
  waiting,
  eating
} State;

typedef struct
{
  int id;
  int hungry;
  State status;
} Customer;