#include <stdio.h>	// include the standard IO library
#include <pthread.h>	// include the pthread library
#include <stdlib.h>	// include the standard general util library

pthread_mutex_t myMutex = PTHREAD_MUTEX_INITIALIZER;	// declare myMutex, the mutex, and
							// initialize it with the macro

typedef struct {	// define myarg_t, a struct type (datatype)
  volatile int counter;	// myarg_t has volatile int counter, the counter
  			// tell the compiler not to optimize anything to do with counter
  int iterations;	// myarg_t has int iterations, the number of iterations
} myarg_t;		// the datatype name


void * thread(void * arg) {	// define thread, the function invoked by a thread, which takes
				// arg, a pointer, and returns a pointer
  myarg_t * myargs = (myarg_t *) arg;	// define myargs, a myarg_t pointer, and point it to the value at arg

  for(int i=0; i < myargs->iterations; i++) {	// iterate (# of iterations in the value at arg) times
    pthread_mutex_lock(&myMutex);	// lock myMutex, block if already locked
    myargs->counter++;	// increment counter in the value at arg each time
    pthread_mutex_unlock(&myMutex);	// unlock myMutex
  }

  return NULL;	// return NULL; terminate the thread
}


int main(int argc, char ** argv) {	// the main function

  int num_threads;	// define int num_threads, the number of threads
  myarg_t myargs;	// define myarg_t myargs, the datatype with counter, iterations


  if( argc != 3 ) {	// if user does not enter 3 command line arguments
    fprintf(stderr, "Usage: ./counter <number of threads> <number of iterations>\n");	// print error msg
    return 1;	// exit with error
  }

  num_threads = atoi(argv[1]);	// convert second cmd line arg to int and pass it to num_threads
  myargs.iterations = atoi(argv[2]);	// convert third cmd line arg to int and pass it to myargs.iterations

  pthread_t threads[num_threads];	// define threads, the array of thread ids of size num_threads
  myargs.counter = 0;	// initialize myargs.counter = 0

  for (int i=0; i<num_threads; i++) {	// for each thread id
    pthread_create(&threads[i], NULL, thread, &myargs);	// create and start a new thread, with thread id
    							// threads[i], that invokes the function thread and
							// passes in the argument myargs
  }

  for (int i = 0; i < num_threads; i++) {	// for each thread id
    pthread_join(threads[i], NULL);	// wait for the specified thread to terminate
  }

  printf("expected counter value: %d\n", myargs.iterations * num_threads);	// print the expected counter value
  printf("counter value: %d\n", myargs.counter);	// print the actual counter value

  return 0;	// exit with success
}
