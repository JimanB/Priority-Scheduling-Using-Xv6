#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc != 3){
    // Use fprintf to print to standard error (file descriptor 2)
    fprintf(2, "Usage: set <pid> <priority>\n");
    // Exit with status 1 to indicate an error
    exit(1);
  }

  int pid = atoi(argv[1]);
  int priority = atoi(argv[2]);

  if(set(pid, priority) < 0){
    // Use fprintf for the error message
    fprintf(2, "Error: set failed. Invalid pid or priority.\n");
    // Exit with status 1
    exit(1);
  }

  // A successful run exits with status 0
  exit(0);
}

/*
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
  if(argc != 3){
    printf(2, "Usage: set <pid> <priority>\n");
    exit();
  }
  // atoi converts the string argument to an integer.
  if(set(atoi(argv[1]), atoi(argv[2])) < 0) {
    printf(2, "Error: set failed. Invalid pid or priority.\n");
  }
  exit();
}
*/
