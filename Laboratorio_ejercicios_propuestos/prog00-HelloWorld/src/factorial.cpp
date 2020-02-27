
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

const int MAX_THREADS = 1024;

long thread_count;
long long n;
double fact;

void* Thread_fact(void* rank);

/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);

int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));
   fact = 1.0;

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_fact, (void*)thread);
/*
   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
*/
   //fact = 4.0*fact;
   printf("With n = %lld terms,\n", n);
   printf("   Our estimate of fact = %.15f\n", fact);

   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:       Thread_fact
 * Purpose:        Add in the terms computed by the thread running this
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count
 * Global in/out:  fact
 */
void* Thread_fact(void* rank) {
   long my_rank = (long) rank;
   long i;
   long my_n = n/thread_count;
   //long long my_first_i = my_n*my_rank;
   //long long my_last_i = my_first_i + my_n;
/*
   if (my_first_i % 2 == 0)
      factor = 1.0;
   else
      factor = -1.0;
*/
   for (i = my_n * my_rank + 1; i <= my_n * (my_rank + 1); i++) {
      fact *= i;
   }

   return NULL;
}  /* Thread_fact */

/*------------------------------------------------------------------
 * Function:   Serial_pi
 * Purpose:    Estimate pi using 1 thread
 * In arg:     n
 * Return val: Estimate of pi using n terms of Maclaurin series
 */
/*
double Serial_pi(long long n) {
   double fact = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      fact += factor/(2*i+1);
   }
   return 4.0*fact;

}
*/
/* Serial_pi */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   n = strtoll(argv[2], NULL, 10);
   if (n <= 0) Usage(argv[0]);
}  /* Get_args */

/*------------------------------------------------------------------
 * Function:  Usage
 * Purpose:   Print a message explaining how to run the program
 * In arg:    prog_name
 */
void Usage(char* prog_name) {
   fprintf(stderr, "usage: %s <number of threads> <n>\n", prog_name);
   fprintf(stderr, "   n is the number of terms and should be >= 1\n");
   fprintf(stderr, "   n should be evenly divisible by the number of threads\n");
   exit(0);
}  /* Usage */