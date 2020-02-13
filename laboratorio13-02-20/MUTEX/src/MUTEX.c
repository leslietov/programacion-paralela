/* File:     pth_pi.c
 * Purpose:  Try to estimate pi using the formula
 *
 *              pi = 4*[1 - 1/3 + 1/5 - 1/7 + 1/9 - . . . ]
 *
 *           This version has a *very serious bug*
 *
 * Compile:  gcc -g -Wall -o pth_pi pth_pi.c -lm -lpthread
 * Run:      ./pth_pi <number of threads> <n>
 *           n is the number of terms of the series to use.
 *           n should be evenly divisible by the number of threads
 * Input:    none
 * Output:   Estimate of pi as computed by multiple threads, estimate
 *           as computed by one thread, and 4*arctan(1).
 *
 * Notes:
 *    1.  The radius of convergence for the series is only 1.  So the
 *        series converges quite slowly.
 *
 * IPP:   Section 4.4 (pp. 162 and ff.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

const int MAX_THREADS = 1024;

long thread_count;
double sum;

pthread_mutex_t mutex;
void* Thread_sum(void* rank);

/* Function we're integrating */
double f(double x);
/* Calculate local integral  */
double Trap(double left_endpt, double right_endpt, int trap_count,
double base_len);
/* Only executed by main thread */
void Get_args(int argc, char* argv[]);
void Usage(char* prog_name);
double Serial_pi(long long n);


double a, b, h;
int n;


int main(int argc, char* argv[]) {
   long       thread;  /* Use long in case of a 64-bit system */
   pthread_t* thread_handles;
   n = 3000;
   a=0.0; b=3.0;
   h = (b-a)/n;

   /* Get number of threads from command line */
   Get_args(argc, argv);

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));
   pthread_mutex_init(&mutex, NULL);
   sum = 0.0;

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
          Thread_sum, (void*)thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);

   //sum = 4.0*sum;
   printf("With n = %d terms,\n", n);
   printf("   Our estimate = %.15f\n", sum);

   pthread_mutex_destroy(&mutex);
   free(thread_handles);
   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:       Thread_sum
 * Purpose:        Add in the terms computed by the thread running this
 * In arg:         rank
 * Ret val:        ignored
 * Globals in:     n, thread_count
 * Global in/out:  sum
 */
void* Thread_sum(void* rank) {
   long my_rank = (long) rank;

   int local_n;
	double local_a, local_b, local_int;
	         /* h is the same for all processes */
   local_n = n/thread_count;  /* So is the number of trapezoids  */

   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
	local_int = Trap(local_a, local_b, local_n, h);
	pthread_mutex_lock(&mutex);
    sum += local_int;
    pthread_mutex_unlock(&mutex);

   return NULL;
}  /* Thread_sum */

/*------------------------------------------------------------------
 * Function:   Serial_pi
 * Purpose:    Estimate pi using 1 thread
 * In arg:     n
 * Return val: Estimate of pi using n terms of Maclaurin series
 */
double Serial_pi(long long n) {
   double sum = 0.0;
   long long i;
   double factor = 1.0;

   for (i = 0; i < n; i++, factor = -factor) {
      sum += factor/(2*i+1);
   }
   return 4.0*sum;

}  /* Serial_pi */

double Trap(
      double left_endpt  /* in */,
      double right_endpt /* in */,
      int    trap_count  /* in */,
      double base_len    /* in */) {
   double estimate, x;
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;
} /*  Trap  */

double f(double x) {
   return x*x;
} /* f */

/*------------------------------------------------------------------
 * Function:    Get_args
 * Purpose:     Get the command line args
 * In args:     argc, argv
 * Globals out: thread_count, n
 */
void Get_args(int argc, char* argv[]) {
  //if (argc != 2) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   if (thread_count <= 0 || thread_count > MAX_THREADS) Usage(argv[0]);
   //n = strtoll(argv[2], NULL, 10);
  //if (n <= 0) Usage(argv[0]);
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
} /* Usage */
