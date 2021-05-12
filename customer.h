typedef enum
{
  waiting,
  eating,
  peeing
} State;

typedef struct
{
  int id;
  int starved;
  int peemergency;
  int usedRestroom;
  State status;
} Customer;