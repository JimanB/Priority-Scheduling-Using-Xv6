/*
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // This volatile is not strictly necessary but prevents the compiler
    // from optimizing the loop away entirely.
    volatile int i;
    for (i = 0; i < 1000000000; i++) {
        // This loop just wastes CPU time to keep the process alive and running.
    }
    exit(0);
}

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    // The 'volatile' keyword helps prevent the compiler from optimizing
    // this loop away. The large number ensures it runs for a while.
    volatile int i;
    for (i = 0; i < 1000000000; i++) {
        // This loop just wastes CPU time.
    }
    exit(0);
}
*/

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    volatile int i, j;
    // Increase the outer loop to make it run much longer
    for (i = 0; i < 5000; i++) {
        for (j = 0; j < 2000000; j++) {
            // Just waste CPU time
        }
    }
    // Let's add a print statement to know when it's done
    fprintf(1, "pi finished for pid %d\n", getpid());
    exit(0);
}
